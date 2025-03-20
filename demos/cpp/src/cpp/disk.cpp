#include "disk.h"
#include "SonDisk.h"
#include "diskUnit.h"
#include "globalValue.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <new>
#include <string>
#include <tuple>
// #include <math.h>

using namespace std;

Disk::Disk() {
  storage.resize(maxDiskSize + 1);
  // setTagDistribute(); 这个set tag 信息不在构造函数中搞了
}

// void Disk::reset() { storage.clear(); }

void Disk::setTagDistribute() {
  // 根据 maxTag 划分磁盘 tag 分区
  int onetagLenth = maxDiskSize / maxTag;
  int last_flag = onetagLenth * maxTag + 1;
  for (int tagId = 1; tagId <= maxTag; tagId++) {
    int flag = 1;
    for (; flag <= onetagLenth; flag++) {
      storage[flag + (tagId - 1) * onetagLenth].tag_id = tagId;
    }
  }
  for (; last_flag <= maxDiskSize; last_flag++) {
    storage[last_flag].tag_id = maxTag;
  }
  // printf("磁盘的tag区间已经划分\n");
}

void Disk::setTagDistribute_v2() {
  vector<tuple<int, int, int>> diskTagInterval = tagDistributeInAllDisk[1];
  // 根据预处理的数据，分配每个 tag 所占用的空间
  // 每个 tag 都有一段固定的区间
  // int tagNum = tagDistributeInAllDisk.size();
  int tagNum = diskTagInterval.size();
  for (int i = 0; i < tagNum; i++) {
    // 它这里的 tag 是从 0 开始的,要加个1
    // 它的存储单元编号也是从 0 开始的
    int tag_id = get<0>(diskTagInterval[i]) + 1;
    int start = get<1>(diskTagInterval[i]) + 1;
    int end = get<2>(diskTagInterval[i]) + 1;
    for (int j = start; j < end; j++) {
      storage[j].tag_id = tag_id;
    }
  }
  int end = get<2>(diskTagInterval[tagNum - 1]) + 1;
  for (; end <= maxDiskSize; end++) {
    storage[end].tag_id = maxTag;
  }
}

void Disk::diskPrintCacheClear() { cache.clear(); }

void Disk::diskDiskHeadInit() { pointer.token = maxToken; }

string Disk::getOncetimeDiskHeadAction() {
  // print cache;
  // for (int i = 0; i < cache.length(); i++) {
  //   printf("%c", cache[i]);
  // }
  // std::getline(std::cin, cache);
  // std::cout << cache;
  // cout.flush();
  return this->cache;
}

void Disk::diskWrite(int unit_id, int obj_id, int obj_size) {
  for (int i = unit_id; i < unit_id + obj_size; i++) {
    if (i > maxDiskSize) { // 如果大于磁盘总存储单元总数
      int newI = realPosition(i);
      storage[newI].object_id = obj_id;
      storage[newI].is_used = true;
    } else {
      storage[i].object_id = obj_id;
      storage[i].is_used = true;
    }
  }
}

void Disk::diskDelete(int unit_id, int obj_size) {
  // int k = 0;
  for (int i = unit_id; i < unit_id + obj_size; i++) {
    // k++;
    // storage[i].is_deleted = true;
    // storage[unit_id].object_block_id = k;
    if (i > maxDiskSize) {
      int newI = realPosition(i);
      storage[newI].object_id = 0;
      storage[newI].is_used = false;
    } else {
      storage[i].object_id = 0;
      storage[i].is_used = false;
    }
  }
}

vector<pair<int, int>> Disk::wherecanput(
    int tag_id) const // 返回所有这个tag的区间, 并且这个区间都是空闲的
{
  // 这里可能没有涉及到判断循环存储单元
  vector<pair<int, int>> res;
  for (int i = 1; i <= maxDiskSize; i++) { // MAX_DISK_SIZE 这个之后应该是 V
    if (storage[i].tag_id == tag_id && storage[i].is_used == false) {
      int start = i;
      while (i <= maxDiskSize && storage[i].tag_id == tag_id &&
             storage[i].is_used == false) {
        i++;
      }
      res.push_back(make_pair(start, i - 1));
    }
  }
  // 按照区间长度 从大到小 排序
  // sort(res.begin(), res.end(),
  //      [](const pair<int, int> &a, const pair<int, int> &b) {
  //        int length_a = a.second - a.first + 1; // 区间长度
  //        int length_b = b.second - b.first + 1; // 区间长度
  //        return length_a > length_b;            // 从大到小排序
  //      });
  return res;
}

// int remainTokens() // 当前磁头剩余 token
int Disk::remainTokens() { return pointer.token; }

// 返回值大于maxToken 即只能 Jump
// path: false: pass过去, true: read过去
// howManyTokensCost 返回读这个 objunit 的 token 预花费数
int Disk::howManyTokensCost(int objUnit, bool &path) {

  int position = pointer.current_position;
  // int distance = objUnit - position; // 有正有负
  int distance = pathLen(position, objUnit);
  int p_cos;
  int r_cos = 0;
  if (distance == 0) {
    // 如果磁头正好在要读存储单元的位置
    if (pointer.pre_is_read == false) {
      path = true;
      return 64;
    } else {
      int pre_token = pointer.pre_token;
      r_cos += max(ceil(pre_token * 0.8), 16);
      // pre_token = max(ceil(pre_token * 0.8), 16);
      path = true;
      return r_cos;
    }
  }

  // 可以read或者pass过去的情况
  else if (distance >= 0 && distance < maxToken) {
    // 计算p_cos的值，一路pass过去的消耗
    p_cos = distance + 64;
    // 计算r_cos的值，一路读过去的消耗
    if (pointer.pre_is_read == true) {
      int pre_token = pointer.pre_token;
      for (int i = 0; i <= distance; i++) {
        r_cos += max(ceil(pre_token * 0.8), 16);
        pre_token = max(ceil(pre_token * 0.8), 16);
      }
    } else if (pointer.pre_is_read == false) {
      int TempTokens = 64;
      for (int i = 0; i <= distance; i++) {
        if (i == 0) {
          r_cos += TempTokens;
          TempTokens = 64;
        } else {
          r_cos += max(ceil(TempTokens * 0.8), 16);
          TempTokens = max(ceil(TempTokens * 0.8), 16);
        }
      }
    }

    int minTempToken = min(p_cos, r_cos);
    (minTempToken == p_cos) ? path = 0 : path = 1;
    return minTempToken;
  } else {
    // 磁头 和 要读位置的距离大于等于 maxToken
    if (pointer.token == maxToken) {
      // 剩余的token 都没有消耗，才能执行跳 的 操作；
      return maxToken + 1;
    }
    // 只能留给下一个时间片处理
    return maxToken + 2;
  }
}

// 当 tokens 没有被使用的时候,且路径长度 大于 token 总数, 那么一定是 jump
// 最快(当前仅当)

bool Disk::diskRead(int unit_id) {
  bool path;
  // 计算读这个unit_id的token预消耗token数量
  int costToken = howManyTokensCost(unit_id, path);
  int curToken = pointer.token;
  if (costToken > curToken) {
    if (costToken == maxToken + 1 && curToken == maxToken) {
      executeJump(unit_id); // 执行跳
    }
    return false;
  }

  // tokenCost > curToken 表示当前时间片读不了，只能先J过去，下个时间片再r
  if (costToken > curToken && curToken == maxToken) {
    // 这个 if 暂时一定进不来
    pointer.token = 0;
    // 就执行了跳的动作了
    pointer.current_position = unit_id;
    // pointer.pre_is_read = false;
    // printf("j");
    cache += "j " + std::to_string(unit_id);
    pointer.pre_is_read = false;
    // pointer.
    return false;
  } else {
    // 当前时间片可以读
    // path: 1: read过去, 0: pass过去
    if (path) {
      pathLen(pointer.current_position, unit_id);
      for (int d = 0; d <= pathLen(pointer.current_position, unit_id); d++) {
        // printf("r");
        if (pointer.pre_is_read == false) {
          // pointer.pre_token = 64;
          if (pointer.token - 64 < 0) {
            pointer.current_position =
                realPosition(pointer.current_position + d);
            return false;
          }
          pointer.token -= 64;
          pointer.pre_is_read = true;
          pointer.pre_token = 64;
        } else {
          if (pointer.token - max(ceil(pointer.pre_token * 0.8), 16) < 0) {
            pointer.current_position =
                realPosition(pointer.current_position + d);
            return false;
          }
          pointer.token -= max(ceil(pointer.pre_token * 0.8), 16);
          pointer.pre_token = max(ceil(pointer.pre_token * 0.8), 16);
          pointer.pre_is_read = true;
        }
        cache += "r";
      }
    } else {
      pathLen(pointer.current_position, unit_id);
      for (int d = 0; d <= (pathLen(pointer.current_position, unit_id) - 1);
           d++) {
        // printf("p");
        if (pointer.token - 1 < 0) {
          pointer.current_position = realPosition(pointer.current_position + d);
          return false;
        }
        cache += "p";
        pointer.token -= 1;
        // pointer.pre_token =
        pointer.pre_is_read = false;
        // pointer.
      }
      if (pointer.token - 64 < 0) {
        pointer.current_position =
            realPosition(pointer.current_position + unit_id);
        return false;
      }
      pointer.token -= 64;
      pointer.pre_is_read = true;
      pointer.pre_token = 64;
      // printf("r");
      cache += "r";
    }
    // pointer.token -= costToken;
    pointer.current_position = realPosition(unit_id + 1);
    return true;
  }
}

bool Disk::executeJump(int unit_id) // 需要执行jump操作
{
  // curerpointer.token
  int current_token = pointer.token;
  if (current_token < maxToken) {
    // 这样就跳不了
    return false;
  }
  pointer.token = 0;
  pointer.current_position = unit_id;
  cache += std::string("j ");
  // std::cout << cache << std::endl;
  cache += std::to_string(unit_id);
  // std::cout << cache << std::endl;
  // cout.flush();
  pointer.pre_is_read = false;
  pointer.pre_token = maxToken;
  return true;
}

int Disk::realPosition(int unit_relative_id) {
  if (unit_relative_id <= maxDiskSize) {
    return unit_relative_id;
  } else {
    return unit_relative_id % maxDiskSize;
  }
}

int Disk::pathLen(int current_pos, int unit_id) {
  if (unit_id >= current_pos) {
    return unit_id - current_pos;
  } else {
    return maxDiskSize - current_pos + unit_id;
  }
}

// NewDisk::NewDisk() { storage.resize(maxDiskSize); }

NewDisk::NewDisk(vector<int> tagVector) {
  // 根据传进来的tag 数，将磁盘按tag分区
  int tagNum = tagVector.size();
  int oneTagLength = maxDiskSize / tagNum;
  int last_flag = oneTagLength * maxTag + 1; // 最后的那个空出来
  sonDiskList.resize(tagNum);
  for (int i = 0; i < tagNum; i++) {
    sonDiskList[i].initSonDisk(tagVector[i], oneTagLength);
  }
}

// int main()

// std::vector<DiskUnit> Disk::getDiskState() { return storage; }

// pair<int, int> Disk::getDiskHead() {
//   pair<int, int> temp;
//   temp.first = pointer.current_position;
//   temp.second = pointer.token;
//   return temp;
// }

// void Disk::diskRead(int action, int unit_id) {
//   if (action == 1) {
//     printf("r");
//   } else if (action == 2) {
//     printf("p");
//   } else if (action == 3) {
//     printf("j");
//   } else if (action == 4) {
//     printf("#");
//   }
//   fflush(stdout);
// }

// vector<pair<int, int>>
// Disk::getStorageUnitIntervalByTag(int tag_id) const //
// 返回所有这个tag的区间
// {
//   vector<pair<int, int>> res;
//   for (int i = 0; i < maxDiskSize; i++) { // MAX_DISK_SIZE 这个之后应该是 V
//     if (storage[i].tag_id == tag_id) {
//       int start = i;
//       while (i < maxDiskSize && storage[i].tag_id == tag_id) {
//         i++;
//       }
//       res.push_back(make_pair(start, i - 1));
//     }
//   }
//   return res;
// }

// vector<pair<int, int>>
// NewDisk::getStorageUnitIntervalByTag(int tag_id) const //
// 返回所有这个tag的区间
// {
//   vector<pair<int, int>> res;
//   for (int i = 0; i < maxDiskSize; i++) { // MAX_DISK_SIZE 这个之后应该是 V
//     if (storage[i].tag_id == tag_id) {
//       int start = i;
//       while (i < maxDiskSize && storage[i].tag_id == tag_id) {
//         i++;
//       }
//       res.push_back(make_pair(start, i - 1));
//     }
//   }
//   return res;
// }

// bool Disk::diskRead(int unit_id) { // read 需要消耗 token 数量
//   if (pointer.token == maxToken &&
//       pathLen(pointer.current_position, unit_id) >= maxToken) {
//     // 如果是这种情况,一定是 jump,
//     并且返回false,这次读没有成功,下一次读的token
//     // 消耗就会变少
//     executeJump(unit_id);
//     return false;
//   }
//   // if()
//   // 也就是我需要在 调用 read 时候一开始就需要 jump
//   int current_pos = pointer.current_position;
//   if (current_pos == unit_id) { // 当前的位置在要读的位置
//     // 我这个当前必然是读
//     if (pointer.read_nums == 0) {
//       pointer.token -= 64; // 如果不够减的话,需要如何操作,必然够减
//       printf("r");
//       pointer.pre_token = 64;
//     } else if (pointer.read_nums != 0) {
//       if (pointer.pre_is_read == false) {
//         // 如果说上一次不是读
//         pointer.token -= 64; //
//         如果不够减的话,需要如何操作,读的时候必然够减 printf("r");
//         pointer.pre_token = 64;
//         // pointer
//       } else if (pointer.pre_is_read == true) {
//         int current_consume_tokens =
//             max(16, ceil((float)pointer.pre_token * 0.8));
//         pointer.token -= current_consume_tokens;
//         printf("r");
//         pointer.pre_token = current_consume_tokens;
//       }
//     }
//   } else if (current_pos != unit_id) {
//     bool flag = false;
//     // 如果当前磁头不在要读的位置
//     //
//     那么需要判断从当前位置到要读位置所需要执行的动作,这里必然要一个一个pass过去
//     if (pathLen(current_pos, unit_id) + 64 > maxToken) {
//       return false;
//     }
//     while (current_pos != unit_id) {
//       // if()
//       pointer.token--;
//       printf("p");
//       current_pos = (current_pos + 1) % maxDiskSize + 1;
//     }
//     printf("r");
//     pointer.token -= 64;
//     pointer.pre_token = 64;
//     // pointer.current_position = (unit_id + 1) % maxDiskSize + 1;
//   }
//   // pointer.pre_token = pointer.token;
//   pointer.current_position = (unit_id + 1) % maxDiskSize + 1;
//   pointer.read_nums++;
//   pointer.pre_is_read = true; // 在磁头不是 read 的时候,需要将这个值改为
//   false return true;
// }

// bool Disk::diskRead(int unit_id) { // 要不这个改为我是否要jump过去
//   int current_pinter_pos = pointer.current_position;
//   int current_token = pointer.token;
//   // if ()
//   // current_token = pointer.token
//   bool keyi_jump = false;
//   bool keyi_pass = false;
//   if (current_token == maxToken) {
//     keyi_jump = true;
//   } else {
//     keyi_jump = false;
//   }
//   // if()
//   int pass_need_token = abs(current_pinter_pos - unit_id);
//   if (current_token >= pass_need_token) {
//     // pointer.token -= pass_need_token;
//     // pointer.current_position = unit_id;
//     // return true;
//     keyi_pass = true;
//   } else {
//     keyi_pass = false;
//   }
//   if (keyi_pass || keyi_jump) {
//     return true;
//   }
// }

// int Disk::howManyTokensCost(int objUnit, int objSize) {
//   int current_pinter_pos = pointer.current_position;
//   int current_token = pointer.token;
//   // int pass_need_token = abs(current_pinter_pos - objUnit);
//   // if (current_token >= pass_need_token) {
//   //   return pass_need_token;
//   // } else {
//   //   return -1;
//   // }
//   bool keyi_jump = false;
//   bool keyi_pass = false;
//   if (current_token == maxToken) {
//     keyi_jump = true;
//   } else {
//     keyi_jump = false;
//   }
//   // if()
//   int pass_need_token = abs(current_pinter_pos - objUnit);
//   if (current_token >= pass_need_token) {
//     // pointer.token -= pass_need_token;
//     // pointer.current_position = unit_id;
//     // return true;
//     // pass_need_token =
//     keyi_pass = true;
//   } else {
//     keyi_pass = false;
//   }
//   if (keyi_pass || keyi_jump) {
//     // return true;
//     if (keyi_jump == true && keyi_pass == false) {
//       return pass_need_token;
//     } else if (keyi_pass) {
//       return pass_need_token;
//     } else {
//       return maxToken;
//     }
//   }
// }

// void Disk::deleteStorageUnit(int Unitindex) {
//   // storage[Unitindex].is_deleted = true;
//   storage[Unitindex].is_used = false;
// }

// DiskUnit Disk::getStorageUnit(int Unitindex) const {
//   return storage[Unitindex];
// }

// void Disk::setStorageUnit(int Unitindex, DiskUnit unit) {
//   storage[Unitindex] = unit;
// }

#include "disk.h"
#include "diskUnit.h"
#include "globalValue.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
// #include <math.h>

using namespace std;

void Disk::reset() { storage.clear(); }

DiskUnit Disk::getStorageUnit(int Unitindex) const {
  return storage[Unitindex];
}

void Disk::setStorageUnit(int Unitindex, DiskUnit unit) {
  storage[Unitindex] = unit;
}

std::vector<DiskUnit> Disk::getDiskState() { return storage; }

void Disk::deleteStorageUnit(int Unitindex) {
  storage[Unitindex].is_deleted = true;
}

void Disk::diskWrite(int unit_id, int obj_id, int obj_size) {
  // storage[unit_id].object_id = obj_id;
  // storage[unit_id].object_block_id = obj_blockid;
  // storage[unit_id].is_deleted = false;
  for (int i = unit_id; i < unit_id + obj_size; i++) {
    storage[i].object_id = obj_id;
    storage[i].object_block_id = i - unit_id + 1;
    storage[i].is_deleted = false;
    storage[i].is_used = true;
  }
}

void Disk::diskDelete(int unit_id, int obj_size) {
  int k = 0;
  for (int i = unit_id; i < unit_id + obj_size; i++) {
    k++;
    storage[i].is_deleted = true;
    storage[unit_id].object_block_id = k;
    storage[i].is_used = false;
  }
}

void Disk::diskRead(int action, int unit_id) {
  if (action == 1) {
    printf("r");
  } else if (action == 2) {
    printf("p");
  } else if (action == 3) {
    printf("j");
  } else if (action == 4) {
    printf("#");
  }
  fflush(stdout);
}

vector<pair<int, int>>
Disk::getStorageUnitIntervalByTag(int tag_id) const // 返回所有这个tag的区间
{
  vector<pair<int, int>> res;
  for (int i = 0; i < MAX_DISK_SIZE; i++) { // MAX_DISK_SIZE 这个之后应该是 V
    if (storage[i].tag_id == tag_id) {
      int start = i;
      while (i < MAX_DISK_SIZE && storage[i].tag_id == tag_id) {
        i++;
      }
      res.push_back(make_pair(start, i - 1));
    }
  }
  return res;
}

vector<pair<int, int>>
NewDisk::getStorageUnitIntervalByTag(int tag_id) const // 返回所有这个tag的区间
{
  vector<pair<int, int>> res;
  for (int i = 0; i < MAX_DISK_SIZE; i++) { // MAX_DISK_SIZE 这个之后应该是 V
    if (storage[i].tag_id == tag_id) {
      int start = i;
      while (i < MAX_DISK_SIZE && storage[i].tag_id == tag_id) {
        i++;
      }
      res.push_back(make_pair(start, i - 1));
    }
  }
  return res;
}

vector<pair<int, int>> Disk::wherecanput(
    int tag_id) const // 返回所有这个tag的区间, 并且这个区间都是空闲的
{
  vector<pair<int, int>> res;
  for (int i = 0; i < MAX_DISK_SIZE; i++) { // MAX_DISK_SIZE 这个之后应该是 V
    if (storage[i].tag_id == tag_id && storage[i].is_used == false) {
      int start = i;
      while (i < MAX_DISK_SIZE && storage[i].tag_id == tag_id &&
             storage[i].is_used == false) {
        i++;
      }
      res.push_back(make_pair(start, i - 1));
    }
  }
  return res;
}

pair<int, int> Disk::getDiskHead() {
  pair<int, int> temp;
  temp.first = pointer.current_position;
  temp.second = pointer.token;
  return temp;
}

// int remainTokens() // 当前磁头剩
int Disk::remainTokens() { return pointer.token; }

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

// 返回值大于maxToken 即只能 Jump
//  path: 0: pass过去, 1: read过去
int Disk::howManyTokensCost(int objUnit,
                            bool &path) { // 读完整个对象,预消耗消耗okens数量
  // int sum_tokens = 0;                       // 预消耗 token 数量
  // // if (pointer.read_nums == 0) {
  // //   sum_tokens += 64; // 第一次读的时候,那么肯定需要消耗 64 个token
  // // } else if (pointer.read_nums != 0) {
  // //   // 如何不是第一次读
  // // }
  // // 我要判断 unit_id 和当前磁头的相对位置
  // int current_pos = pointer.current_position;
  // int len = pathLen(current_pos, unit_id);
  // if (len == 0) {
  //   //
  // }
  int position = pointer.current_position;
  // int = pathLen(position, objUnit);
  int distance = objUnit - position;
  int p_cos;
  int r_cos = 0;

  if (distance > 0 && distance < maxToken) {

    p_cos = distance + 64;

    // for(int i= 0; i < distance; i++) {

    // }
    if (pointer.pre_is_read == true) {

      int pre_token = pointer.pre_token;
      for (int i = 0; i < distance; i++) {
        r_cos += min(pre_token * 0.8, 16);
        pre_token = min(pre_token * 0.8, 16);
      }
    }

    int minToken = min(p_cos, r_cos);
    (minToken == p_cos) ? path = 0 : path = 1;
    return minToken;
  }
  return maxToken + 1;
}

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

// 当 tokens 没有被使用的时候,且路径长度 大于 token 总数, 那么一定是 jump
// 最快(当前仅当)

bool Disk::diskRead(int unit_id) {
  bool path;
  int tokenCost = howManyTokensCost(unit_id, path);
  if (tokenCost > maxToken) {
    return false;
  } else {
    if (path) {
      for (int d = 0; d < (unit_id - pointer.current_position); d++) {
        printf("r");
      }
    } else {
      for (int d = 0; d < (unit_id - pointer.current_position - 1); d++) {
        printf("p");
      }
      printf("r");
    }
    pointer.token -= tokenCost;
    pointer.current_position = unit_id;
    return true;
  }
}

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
//         pointer.token -= 64; // 如果不够减的话,需要如何操作,读的时候必然够减
//         printf("r");
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

bool Disk::executeJump(int unit_id) // 需要执行jump操作
{
  // curerpointer.token
  int current_token = pointer.token;
  if (current_token < maxToken) {
    return false;
  }
  pointer.current_position = unit_id;
  pointer.pre_is_read = false;
  pointer.pre_token = maxToken;
  pointer.token = maxToken;
  return true;
}
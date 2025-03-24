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
  freeUnitSize = maxDiskSize;
  freeMaxTagIntervalSize = 0;
  storage.resize(maxDiskSize + 1);
}

int Disk::func_add_rcos(int distance, bool pre_isornot_read) {
  int r_cos = 0;
  int pre_token = pointer.pre_token;
  if (pre_isornot_read == false) {
    if (distance > 0 && distance <= 8) {
      r_cos += RCOS[distance];
    } else {
      r_cos += (distance - 8) * 16;
    }
  }
  return r_cos;
}

void Disk::setTagDistribute_v2() {
  vector<tuple<int, int, int>> diskTagInterval = tagDistributeInAllDisk[1];
  // 把每个 tag 在磁盘上所占用的区间，push 到 tag_interval 中
  // int tag_id;
  int start;
  int end;
  for (int i = 0; i < diskTagInterval.size() - 1; i++) {
    // tag_id = get<0>(diskTagInterval[i]) + 1;
    start = get<1>(diskTagInterval[i]) + 1;
    end = get<2>(diskTagInterval[i]) + 1;
    // 放入的区间是 [ start, end )
    // tag 从 0 开始 [0, 15]
    tag_interval.emplace_back(make_pair(start, end));
  }
  start = end;
  end = maxDiskSize + 1;
  // 放入的区间是 [ start, end )
  tag_interval.emplace_back(make_pair(start, end));
  // 初始化的时候，一开始全是空闲空间， 放到 free_interval 中
  // for (int i = 0; i < tag_interval.size(); i++) {
  //   // i 代表的就是第几个 tag
  //   free_interval.emplace_back(vector<pair<int, int>>());
  //   free_interval[i].emplace_back(tag_interval[i]);
  // }
}

tuple<int, int, int> Disk::getfreeMaxTagIntervalSize() {
  tuple<int, int, int> temp;
  get<0>(temp) = freeMaxTagIntervalSize;
  get<1>(temp) = freeMaxTagIntervalStart;
  get<2>(temp) = freeMaxTagIntervalEnd;
  return temp;
}

void Disk::diskPrintCacheClear() { cache.clear(); }

void Disk::diskDiskHeadInit() { pointer.token = maxToken; }

bool Disk::isNotNum_func(char c) {
  if (c >= '0' && c <= '9') {
    return false;
  }
  return true;
}

void Disk::getOncetimeDiskHeadAction() {
  if (cache.size() == 0) {
    cout << "#";
  } else {
    if (cache[0] == 'j') {
      cout << "j ";
      string temp;
      int num = 0;
      for (int i = 2; i < cache.size(); i++) {
        temp += cache[i];
      }
      num = std::stoi(temp);
      cout << num;
    } else {
      cout << cache << "#";
    }
  }
}

void Disk::diskWrite(int unit_id, int obj_id, int obj_size) {
  freeUnitSize -= obj_size;
  for (int i = unit_id; i < unit_id + obj_size; i++) {
    if (i > maxDiskSize) { // 如果大于磁盘总存储单元总数
      int newI = realPosition(i);
      // storage[newI].object_id = obj_id;
      storage[newI].is_used = true;
    } else {
      // storage[i].object_id = obj_id;
      storage[i].is_used = true;
    }
  }
}
void Disk::diskWrite(int unit_id, int obj_id, int obj_size, int tag_id) {
  for (int i = unit_id; i < unit_id + obj_size; i++) {
    if (i > maxDiskSize) { // 如果大于磁盘总存储单元总数
      int newI = realPosition(i);
      // storage[newI].object_id = obj_id;
      storage[newI].is_used = true;
    } else {
      // storage[i].object_id = obj_id;
      storage[i].is_used = true;
    }
  }
  // 这里知道要写入的区间
  // 动态修改 空闲区间 [start, end)
  int allocated_start = unit_id;
  int allocated_end = unit_id + obj_size;
  vector<pair<int, int>> &free_space = free_interval[tag_id - 1];
  for (int i = 0; i < free_space.size(); i++) {
    int start = free_space[i].first;
    int end = free_space[i].second;
    if (allocated_start >= end || allocated_end <= start) {
      continue;
    }
    if (allocated_start <= start && allocated_end >= end) {
      free_space.erase(free_space.begin() + i);
      i--;
    } else if (allocated_start <= start && allocated_end < end) {
      free_space[i].first = allocated_end;
    } else if (allocated_start > start && allocated_end >= end) {
      free_space[i].second = allocated_start;
    } else {
      free_space.emplace_back(make_pair(allocated_end, end));
      free_space[i].second = allocated_start;
    }
  }
}

void Disk::diskDelete(int unit_id, int obj_size) {
  freeUnitSize += obj_size;
  // int k = 0;
  for (int i = unit_id; i < unit_id + obj_size; i++) {
    // k++;
    // storage[i].is_deleted = true;
    // storage[unit_id].object_block_id = k;
    if (i > maxDiskSize) {
      int newI = realPosition(i);
      // storage[newI].object_id = 0;
      storage[newI].is_used = false;
    } else {
      // storage[i].object_id = 0;
      storage[i].is_used = false;
    }
  }
}

void Disk::diskDelete(int unit_id, int obj_size, int tag_id) {
  // int k = 0;
  for (int i = unit_id; i < unit_id + obj_size; i++) {
    // k++;
    // storage[i].is_deleted = true;
    // storage[unit_id].object_block_id = k;
    if (i > maxDiskSize) {
      int newI = realPosition(i);
      // storage[newI].object_id = 0;
      storage[newI].is_used = false;
    } else {
      // storage[i].object_id = 0;
      storage[i].is_used = false;
    }
  }

  // 这里知道要删除的区间，删除过后就变成空闲空间了
  // 动态修改 空闲区间 [start, end)
  int deleted_start = unit_id;
  int deleted_end = unit_id + obj_size;
  vector<pair<int, int>> &free_space = free_interval[tag_id - 1];
  // 合并或插入新的空闲区间
  // 如果free_space为空，直接插入新区间

  // 检查新的空闲区间是否可以与现有区间合并
  bool merged = false;

  // 遍历现有空闲区间列表
  for (int i = 0; i < free_space.size(); i++) {
    // 检查是否可以合并 - 两个区间相邻或重叠
    // 区间1：[free_space[i].first, free_space[i].second)
    // 区间2：[deleted_start, deleted_end)
    // 两个区间相邻或重叠的条件：一个区间的start等于另一个区间的end，或者有重叠
    if (free_space[i].first <= deleted_end &&
        deleted_start <= free_space[i].second) {
      // 合并区间
      free_space[i].first = min(free_space[i].first, deleted_start);
      free_space[i].second = max(free_space[i].second, deleted_end);
      merged = true;

      // 继续检查其他区间是否也能合并到这个新合并的区间
      for (int j = i + 1; j < free_space.size();) {
        if (free_space[j].first <= free_space[i].second &&
            free_space[i].first <= free_space[j].second) {
          // 可以与当前合并后的区间合并
          free_space[i].first = min(free_space[i].first, free_space[j].first);
          free_space[i].second =
              max(free_space[i].second, free_space[j].second);

          // 移除已经合并的区间
          free_space.erase(free_space.begin() + j);
        } else {
          j++; // 只有当区间不能合并时才增加j
        }
      }

      break; // 已经合并到一个区间，不需要继续检查
    }
  }

  // 如果没有合并到任何现有区间，则添加为新区间
  if (!merged) {
    free_space.push_back({deleted_start, deleted_end});
  }
}
vector<pair<int, int>>
Disk::wherecanput(int tag_id) // 返回所有这个tag的区间, 并且这个区间都是空闲的
{
  // v2 版本 运行时间减少 16 倍,但实际上没有减少
  vector<pair<int, int>> res;
  freeMaxTagIntervalSize = 0;
  freeMaxTagIntervalStart = 0;
  freeMaxTagIntervalEnd = 0;
  //
  int tag_id_new = tag_id - 1;
  int start_sunwei = tag_interval[tag_id_new].first;
  int end_sunwei = tag_interval[tag_id_new].second - 1;
  for (int i = start_sunwei; i <= end_sunwei; i++) {
    if (storage[i].is_used == false) {
      int start = i;
      while (i <= end_sunwei && storage[i].is_used == false) {
        i++;
      }
      // 返回的是 [start, i-1]
      if (i - start > freeMaxTagIntervalSize) {
        freeMaxTagIntervalSize = i - start;
        freeMaxTagIntervalStart = start;
        freeMaxTagIntervalEnd = i - 1;
      }
      res.emplace_back(make_pair(start, i - 1));
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

// vector<pair<int, int>> Disk::wherecanput(
//     int tag_id) const // 返回所有这个tag的区间, 并且这个区间都是空闲的
// {
//   vector<pair<int, int>> res = free_interval[tag_id - 1];
//   return res;
// }

// int remainTokens() // 当前磁头剩余 token
int Disk::remainTokens() { return pointer.token; }

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
  else if (distance > 0 && distance <= maxToken - 64) { // 这里的 <= 会影响分数
    // 计算p_cos的值，一路pass过去的消耗
    p_cos = distance + 64;
    path = 0;
    return p_cos;
  } else {
    // 磁头 和 要读位置的距离大于等于 maxToken
    if (pointer.token == maxToken) {
      // 剩余的token 都没有消耗，才能执行跳 的 操作；
      return maxToken + 1;
    }
    // 能执行到这里说明想要跳但是当前时间片有消耗，就没办法跳，不能进行操作
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
    // 预消耗 token 大于 当前 token
    if (costToken == maxToken + 1 && curToken == maxToken) {
      // 想要跳剩余token数也够跳，能拿到 maxToken + 1
      // ，也就说明了一定能够跳，剩余token量一定是maxtoken
      executeJump(unit_id); // 执行跳
      return false;
    } else if (costToken == maxToken + 2) {
      // 想要跳但是当前时间片的token有消耗就不够跳，那么就不能执行跳的动作
      return false;
    } else {
      // 在当前磁盘上的剩余token不够任何操作，磁头没有任何动作，读取不成功
      return false;
    }
  }

  // tokenCost > curToken 表示当前时间片读不了，只能先J过去，下个时间片再r
  // 如果能执行到这里，那么说明，剩余token数量，肯定是能够读到想要读的那个存储单元的
  // 磁头一定会有动作
  // 返回值一定会是 true
  // path: 1: read过去读, 0: pass过去读
  if (path) {
    // for (int d = 0; d <= pathLen(pointer.current_position, unit_id); d++) {
    if (pointer.pre_is_read == false) {
      pointer.token -= 64;
      pointer.pre_is_read = true;
      pointer.pre_token = 64;
    } else {
      pointer.token -= max(ceil(pointer.pre_token * 0.8), 16);
      pointer.pre_token = max(ceil(pointer.pre_token * 0.8), 16);
      pointer.pre_is_read = true;
    }
    cache += "r";
    // }
  } else {
    int dist = pathLen(pointer.current_position, unit_id);
    for (int d = 0; d <= (dist - 1); d++) {
      cache += "p";
      // pointer.token -= 1;
    }
    // 两种加字符串的方式到底哪个快，还有待考究
    // cache.append(pathLen(pointer.current_position, unit_id), 'p');
    // pointer.token -= 64;
    pointer.token -= costToken;
    pointer.pre_is_read = true;
    pointer.pre_token = 64;
    cache += "r";
  }
  pointer.current_position = realPosition(unit_id + 1);
  return true;
}

bool Disk::executeJump(int unit_id) // 需要执行jump操作
{
  int current_token = pointer.token;
  if (current_token < maxToken) {
    return false;
  }
  pointer.token = 0;
  pointer.current_position = unit_id;
  cache += std::string("j ");
  cache += std::to_string(unit_id);
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
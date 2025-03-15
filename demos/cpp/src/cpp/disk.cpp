#include "disk.h"
#include "diskUnit.h"
#include "globalValue.h"
#include <cstdlib>

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

bool Disk::diskRead(int unit_id) {
  int current_pinter_pos = pointer.current_position;
  int current_token = pointer.token;
  // if ()
  // current_token = pointer.token
  bool keyi_jump = false;
  bool keyi_pass = false;
  if (current_token == maxToken) {
    keyi_jump = true;
  } else {
    keyi_jump = false;
  }
  // if()
  int pass_need_token = abs(current_pinter_pos - unit_id);
  if (current_token >= pass_need_token) {
    // pointer.token -= pass_need_token;
    // pointer.current_position = unit_id;
    // return true;
    keyi_pass = true;
  } else {
    keyi_pass = false;
  }
  if (keyi_pass || keyi_jump) {
    return true;
  }
}

int Disk::howManyTokensCost(int objUnit, int objSize) {
  int current_pinter_pos = pointer.current_position;
  int current_token = pointer.token;
  // int pass_need_token = abs(current_pinter_pos - objUnit);
  // if (current_token >= pass_need_token) {
  //   return pass_need_token;
  // } else {
  //   return -1;
  // }
  bool keyi_jump = false;
  bool keyi_pass = false;
  if (current_token == maxToken) {
    keyi_jump = true;
  } else {
    keyi_jump = false;
  }
  // if()
  int pass_need_token = abs(current_pinter_pos - objUnit);
  if (current_token >= pass_need_token) {
    // pointer.token -= pass_need_token;
    // pointer.current_position = unit_id;
    // return true;
    // pass_need_token =
    keyi_pass = true;
  } else {
    keyi_pass = false;
  }
  if (keyi_pass || keyi_jump) {
    // return true;
    if (keyi_jump == true && keyi_pass == false) {
      return pass_need_token;
    } else if (keyi_pass) {
      return pass_need_token;
    } else {
      return maxToken;
    }
  }
}
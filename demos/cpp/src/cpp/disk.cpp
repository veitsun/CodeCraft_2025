#include "disk.h"
#include "diskUnit.h"

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

void Disk::writeValue(int unit_id, int obj_id, int obj_blockid) {
  storage[unit_id].object_id = obj_id;
  storage[unit_id].object_block_id = obj_blockid;
  storage[unit_id].is_deleted = false;
}

void Disk::deleteValue(int unit_id) { storage[unit_id].is_deleted = true; }

void Disk::readValue(int action, int unit_id) {
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
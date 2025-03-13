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
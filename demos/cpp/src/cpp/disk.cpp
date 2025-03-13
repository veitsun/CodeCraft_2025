#include "disk.h"

using namespace std;

// void Disk::Disk() {}

void Disk::reset() {
  for (int i = 0; i < MAX_DISK_SIZE; i++) {
    storage[i].clear();
  }
}

// int Disk::getDiskId() const { return diskId; }

// void Disk::setDiskId(int id) { diskId = id; }

// std::vector<void *> Disk::getStorageValue(int Unitindex) const {
//   return storage[Unitindex];
// }

void Disk::setStorageValue(int Unitindex, const std::vector<void *> &value) {
  storage[Unitindex] = value;
}

std::vector<void *> *Disk::getStorageArray() { return storage; }

void Disk::deleteStorageValue(int Unitindex) { storage[Unitindex].clear(); }

void Disk::writeValue(int unit_id, int obj_id, int obj_blockid) {
  storage[unit_id].push_back(nullptr);
}

void Disk::deleteValue(int unit_id) { storage[unit_id].clear(); }

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
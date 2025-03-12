#include "disk.h"

using namespace std;

void Disk::reset() {
  for (int i = 0; i < MAX_DISK_SIZE; i++) {
    storage[i].clear();
  }
}

int Disk::getDiskId() const { return diskId; }

void Disk::setDiskId(int id) { diskId = id; }

std::vector<void *> Disk::getStorageValue(int Unitindex) const {
  return storage[Unitindex];
}

void Disk::setStorageValue(int Unitindex, const std::vector<void *> &value) {
  storage[Unitindex] = value;
}

std::vector<void *> *Disk::getStorageArray() { return storage; }
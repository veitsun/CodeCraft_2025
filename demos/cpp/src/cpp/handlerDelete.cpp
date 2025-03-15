#include "handlerDelete.h"
#include "disk.h"

Disk disk;
void handlerdelete::handlerDelete2Disk(int unitId, int size) {
  disk.diskDelete(unitId, size);
}
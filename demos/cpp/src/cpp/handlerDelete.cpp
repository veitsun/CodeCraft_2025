#include "handlerDelete.h"
#include "disk.h"

Disk disk;
void handlerdelete::handlerDelete2Disk(int unitId, int objId, int size) {
  disk.diskWrite(unitId, objId, size);
}
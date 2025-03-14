#include "handlerRead.h"
#include "disk.h"
Disk disk;
void handlerread::handlerRead2Disk(int unit_id, int obj_size) {
  disk.diskDelete(unit_id, obj_size);
}
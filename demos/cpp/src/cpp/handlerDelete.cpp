#include "handlerDelete.h"
#include "disk.h"
#include "globalValue.h"
#include "request.h"
#include <cstring>
#include <vector>

// Disk disk;
// V1暂时没用这个函数
// void handlerdelete::handlerDelete2Disk(int unitId, int size) {
//   disk.diskDelete(unitId, size);
// }

bool handlerdelete::handlerRequestfromScheduler(delRequest delRequest) {
  bool isDone = false;
  Object obj = object_list.getObject(delRequest.getObjectId());
  int objSize = obj.getObjectSize();
  vector<int> diskIndx = obj.getObjectDisk();
  vector<int> objUnit = obj.getObjectUnit();
  for (int i{0}; i < REP_NUM; i++) {
    diskList[diskIndx[i]].diskDelete(objUnit[i], objSize);
  }

  return isDone;
}
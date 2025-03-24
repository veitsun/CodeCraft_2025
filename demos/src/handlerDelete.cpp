#include "handlerDelete.h"
#include "disk.h"
#include "globalValue.h"
#include "request.h"
#include <cstring>
#include <vector>

bool handlerdelete::handlerRequestfromScheduler(delRequest delRequest) {
  bool isDone = false;
  Object obj = object_list.getObject(delRequest.getObjectId());
  int objSize = obj.getObjectSize();
  vector<int> diskIndx = obj.getObjectDisk();
  vector<int> objUnit = obj.getObjectUnit();
  for (int i{0}; i < REP_NUM; i++) {
    diskList[diskIndx[i] - 1].diskDelete(objUnit[i], objSize);
  }

  return isDone;
}

void handlerdelete::printCompleteRequest() {}

#include "handlerRead.h"
#include "globalValue.h"
#include "object.h"
#include "request.h"
#include <cstdio>
#include <cstring>
#include <vector>

// void handlerread::handlerRead2Disk(int unit_id, int obj_size) {
//   disk.diskDelete(unit_id, obj_size);
// }

// 现在的行为是读一个对象
bool handlerread::handlerRequestfromScheduler(readRequest readRequest) {
  bool whoever;
  bool isDone = false;

  int costTokens;
  // int objUnit[REP_NUM];
  Object obj = object_list.getObject(readRequest.getObjectId());
  vector<int> objUnit = obj.getObjectUnit();
  vector<int> objDisk = obj.getObjectDisk();
  int objSize = obj.getObjectSize();
  // 我现在每一个副本写的位置都是一样的，每个副本写在不同的磁盘，每个副本在各自磁盘的位置相同，所以我现在读，只需要读一个副本即可，虽然他这里给的unit是三个副本的数据，但是每个副本的数据都是一样的，所以我用第一个就ok了
  // 遍历这个对象的对象块
  // 如果当前磁头预消耗tokens小于当前磁头剩余tokens数量，一个if的行为对应读一个对象的对象块
  for (int rep_num = 0; rep_num < objDisk.size(); rep_num++) {
    int diskID = objDisk[rep_num];
    if ((diskList[diskID].howManyTokensCost(objUnit[0], whoever)) <
        diskList[diskID].remainTokens()) {
      // 可以读
      for (int i{0}; i < objSize; i++) {
        diskList[diskID].diskRead(objUnit[i]);
      }
      // 先假设我的决策一定是正确的
      isDone = true;
      completeRequest.push_back(readRequest.getRequestId());
      break;
    }
  }

  return isDone;
}

void handlerread::printCompleteRequest() {
  printf("%d/n", completeRequest.size());
  for (int i = 0; i < completeRequest.size(); i++) {
    printf("%d/n", completeRequest[i]);
  }
  return;
}

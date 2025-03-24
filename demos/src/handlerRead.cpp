#include "handlerRead.h"
#include "globalValue.h"
#include "object.h"
#include "request.h"
#include <cstdio>
#include <cstring>
#include <vector>

// 现在的行为是读一个对象
// 不跨时间片读
// 第一个参数是是否读成功，第二参数是读了多少个对象块，第三个参数是这次读的磁盘号，第四个参数是起始Unit位置，第五个参数是如果这个请求要做未做就true
tuple<bool, int, int, int, bool>
handlerread::handlerRequestfromScheduler(readRequest readRequest) {
  bool whoever;
  tuple<bool, int, int, int, bool> isDone;
  std::get<0>(isDone) = false;
  std::get<4>(isDone) = false;

  int costTokens;
  // int objUnit[REP_NUM];
  Object obj = object_list.getObject(readRequest.getObjectId());
  vector<int> objUnit = obj.getObjectUnit();
  vector<int> repDisk = obj.getObjectDisk();
  int objSize = obj.getObjectSize();

  // bool judge = true;
  bool judge = false;
  // 遍历这个对象的对象块
  // 如果当前磁头预消耗tokens小于当前磁头剩余tokens数量，一个if的行为对应读一个对象的对象块
  for (int i = 0; i < REP_NUM; i++) {
    int checkHowManyTokensCost =
        (diskList[repDisk[i] - 1].howManyTokensCost(objUnit[i], whoever));
    /****************************************要做已做**************************************/
    /*************************（包括要跳已跳要读已读或者要读未读完）**************************/
    // 跳逻辑
    if (checkHowManyTokensCost == maxToken + 1) {
      whichDiskIJumped.emplace_back(repDisk[i] - 1);
      // 跳本来应该返回false
      std::get<0>(isDone) = diskList[repDisk[i] - 1].diskRead(objUnit[i]);
      // 如果跳了，就说明一个obj的unit都没读到
      std::get<1>(isDone) = 0;
      std::get<2>(isDone) = repDisk[i];
      std::get<3>(isDone) = objUnit[i];
      break;
    }

    // 最少可以读一个
    int checkRemainTokens = diskList[repDisk[i] - 1].remainTokens();
    if ((diskList[repDisk[i] - 1].howManyTokensCost(objUnit[i], whoever)) <=
        diskList[repDisk[i] - 1].remainTokens()) {
      // 可以读,如何判断读了多少个并且记录到isDone中
      for (int j{0}; j < objSize; j++) {
        judge = diskList[repDisk[i] - 1].diskRead(objUnit[i] + j);
        if (!judge) {
          // 这里是如果读失败了，就记录读成功了多少个unit到isDone
          std::get<1>(isDone) = j;
          std::get<2>(isDone) = repDisk[i];
          std::get<3>(isDone) = objUnit[i];
          // 如果读失败了，确实可以直接在这里直接修改obj的unitid和size用于下一个时间片接着读
          break;
        }
      }
      if (judge == false) {
        std::get<0>(isDone) = false;
      } else {
        std::get<0>(isDone) = true;
        completeRequest.emplace_back(readRequest.getRequestId());
      }
      break;
    }

    /****************************************要做未做**************************************/
    /*******************************（包括要跳未跳或者要读未读）*****************************/
    else if ((i == 2) && ((diskList[repDisk[i] - 1].howManyTokensCost(
                              objUnit[i], whoever)) >
                          diskList[repDisk[i] - 1].remainTokens())) {

      /*
      这里包括了 checkHowManyTokensCost == maxToken + 2的情况(要跳未跳)
      这里包括了(diskList[repDisk[i] - 1].howManyTokensCost(objUnit[i],
      whoever)) > diskList[repDisk[i] - 1].remainTokens()（要读未读）
      */
      std::get<4>(isDone) = true;
      break;
    }
  }

  return isDone;
}

// 先只看跳和没读完，先不看完全没读
/*
readRequest是为了存请求号并且存到complete里面去用于读成功的输出
objUnitFromOnce现在是因为三副本的unitId不一样，所以需要指定unitId
objSizeFromOnce要读多大是一定要给的
objDiskId在哪个磁盘上读也是一定需要的
 */
pair<bool, int>
handlerread::handlerRequestfromScheduler(readRequest readRequest,
                                         int objUnitFromOnce,
                                         int objSizeFromOnce, int objDiskId) {
  pair<bool, int> isDone;
  isDone.first = false;
  int objUnit = objUnitFromOnce;
  int objSize = objSizeFromOnce;
  int diskID = objDiskId;

  bool whoever;
  int checkHowManyTokensCost =
      (diskList[objDiskId - 1].howManyTokensCost(objUnitFromOnce, whoever));
  int checkRemainTokens = diskList[objDiskId - 1].remainTokens();

  if ((checkHowManyTokensCost == maxToken + 2) ||
      (checkHowManyTokensCost == maxToken + 1)) {
    isDone.second = -1;
    return isDone;
  }

  for (int j{0}; j < objSize; j++) {
    isDone.first = diskList[diskID - 1].diskRead(objUnit + j);
    if (!isDone.first) {
      isDone.second = j;
      break;
    }
  }
  // std::get<0>(isDone) = true;
  if (isDone.first)
    completeRequest.emplace_back(readRequest.getRequestId());
  return isDone;
}

void handlerread::printCompleteRequest() {
  printf("%d\n", completeRequest.size());
  for (int i = 0; i < completeRequest.size(); i++) {
    printf("%d\n", completeRequest[i]);
  }
  return;
}

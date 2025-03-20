#include "handlerRead.h"
#include "globalValue.h"
#include "object.h"
#include "request.h"
#include <cstdio>
#include <cstring>
#include <vector>

// 现在的行为是读一个对象
// 不跨时间片读
tuple<bool, int, int>
handlerread::handlerRequestfromScheduler(readRequest readRequest) {
  bool whoever;
  tuple<bool, int, int> isDone;
  std::get<0>(isDone) = false;

  int costTokens;
  // int objUnit[REP_NUM];
  Object obj = object_list.getObject(readRequest.getObjectId());
  vector<int> objUnit = obj.getObjectUnit();
  vector<int> repDisk = obj.getObjectDisk();
  int objSize = obj.getObjectSize();

  // bool judge = true;
  bool judge = false;
  // 我现在每一个副本写的位置都是一样的，每个副本写在不同的磁盘，每个副本在各自磁盘的位置相同，所以我现在读，只需要读一个副本即可，虽然他这里给的unit是三个副本的数据，但是每个副本的数据都是一样的，所以我用第一个就ok了
  // 遍历这个对象的对象块
  // 如果当前磁头预消耗tokens小于当前磁头剩余tokens数量，一个if的行为对应读一个对象的对象块
  for (int i = 0; i < repDisk.size(); i++) {
    int diskID = repDisk[i];
    int checkHowManyTokensCost =
        (diskList[diskID - 1].howManyTokensCost(objUnit[0], whoever));
    // 跳逻辑
    if (checkHowManyTokensCost == maxToken + 1) {
      whichDiskIJumped.emplace_back(diskID - 1);
      // 跳本来应该返回false
      std::get<0>(isDone) = diskList[diskID - 1].diskRead(objUnit[0]);
      // 如果跳了，就说明一个obj的unit都没读到
      std::get<1>(isDone) = 0;
      std::get<2>(isDone) = diskID;
      break;
    }
    // 这个逻辑里面不存在跳操作
    // if (checkHowManyTokensCost >= 101) {
    //   readFailureForJump = diskID - 1;
    // }
    int checkRemainTokens = diskList[diskID - 1].remainTokens();
    if ((diskList[diskID - 1].howManyTokensCost(objUnit[0], whoever)) <
        diskList[diskID - 1].remainTokens()) {
      // 可以读,如何判断读了多少个并且记录到isDone中
      for (int j{0}; j < objSize; j++) {
        judge = diskList[diskID - 1].diskRead(objUnit[j] + j);
        if (!judge) {
          // 这里是如果读失败了，就记录读成功了多少个unit到isDone
          std::get<1>(isDone) = j;
          std::get<2>(isDone) = diskID;
          // 如果读失败了，确实可以直接在这里直接修改obj的unitid和size用于下一个时间片接着读
          break;
        }
      }
      if (judge == false) {
        // 如果没读到（一定是j才没读到），就返回这个磁盘的id,这里的readFailureForToken是暂时还没有用的，但是也是可以的
        readFailureForToken = diskID - 1;
      } else {
        std::get<0>(isDone) = true;
        completeRequest.emplace_back(readRequest.getRequestId());
      }
      break;
    }
  }

  return isDone;
}

// 用于跨时间片读，第二次读，第二次读是不可能读失败的
pair<bool, int>
handlerread::handlerRequestfromScheduler(readRequest readRequest,
                                         vector<int> objUnitFromOnce,
                                         int objSizeFromOnce, int objDiskId) {
  bool whoever;
  pair<bool, int> isDone;
  std::get<0>(isDone) = false;

  int costTokens;
  // int objUnit[REP_NUM];
  Object obj = object_list.getObject(readRequest.getObjectId());
  vector<int> objUnit = objUnitFromOnce;
  vector<int> repDisk = obj.getObjectDisk();
  int objSize = objSizeFromOnce;

  // bool judge = true;
  bool judge = false;
  // 我现在每一个副本写的位置都是一样的，每个副本写在不同的磁盘，每个副本在各自磁盘的位置相同，所以我现在读，只需要读一个副本即可，虽然他这里给的unit是三个副本的数据，但是每个副本的数据都是一样的，所以我用第一个就ok了
  // 遍历这个对象的对象块
  // 如果当前磁头预消耗tokens小于当前磁头剩余tokens数量，一个if的行为对应读一个对象的对象块
  for (int i = 0; i < repDisk.size(); i++) {
    int diskID = repDisk[i];
    if (objDiskId != -1) {
      diskID = objDiskId;
    }

    int checkHowManyTokensCost =
        (diskList[diskID - 1].howManyTokensCost(objUnit[0], whoever));
    if (checkHowManyTokensCost == maxToken + 1) {
      whichDiskIJumped.emplace_back(diskID - 1);
      std::get<0>(isDone) = diskList[diskID - 1].diskRead(objUnit[0]);
      std::get<1>(isDone) = 0;
      break;
    }
    // 这个逻辑里面不存在跳操作
    // if (checkHowManyTokensCost >= 101) {
    //   readFailureForJump = diskID - 1;
    // }
    int checkRemainTokens = diskList[diskID - 1].remainTokens();
    if ((diskList[diskID - 1].howManyTokensCost(objUnit[0], whoever)) <
        diskList[diskID - 1].remainTokens()) {
      // 可以读,如何判断读了多少个并且记录到isDone中
      for (int j{0}; j < objSize; j++) {
        judge = diskList[diskID - 1].diskRead(objUnit[j] + j);
        if (!judge) {
          // 这里是如果读失败了，就记录读成功了多少个unit到isDone
          std::get<1>(isDone) = j;
          // 如果读失败了，确实可以直接在这里直接修改obj的unitid和size用于下一个时间片接着读
          break;
        }
      }
      if (judge == false) {
        // 如果没读到（一定是j才没读到），就返回这个磁盘的id,这里的readFailureForToken是暂时还没有用的，但是也是可以的
        readFailureForToken = diskID - 1;
      } else {
        std::get<0>(isDone) = true;
        completeRequest.emplace_back(readRequest.getRequestId());
      }
      break;
    }
  }

  return isDone;
}

void handlerread::printCompleteRequest() {
  printf("%d\n", completeRequest.size());
  for (int i = 0; i < completeRequest.size(); i++) {
    printf("%d\n", completeRequest[i]);
  }
  return;
}

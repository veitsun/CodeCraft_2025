#include "handlerRead.h"
#include "globalValue.h"
#include "object.h"
#include "request.h"
#include <cstdio>
#include <cstring>
#include <vector>

// 现在的行为是读一个对象
pair<bool, int>
handlerread::handlerRequestfromScheduler(readRequest readRequest) {
  bool whoever;
  pair<bool, int> isDone;
  isDone.first = false;

  int costTokens;
  // int objUnit[REP_NUM];
  Object obj = object_list.getObject(readRequest.getObjectId());
  vector<int> objUnit = obj.getObjectUnit();
  vector<int> repDisk = obj.getObjectDisk();
  int objSize = obj.getObjectSize();

  bool judge = true;
  vector<int> setObjUnit;
  setObjUnit.resize(3);
  // 我现在每一个副本写的位置都是一样的，每个副本写在不同的磁盘，每个副本在各自磁盘的位置相同，所以我现在读，只需要读一个副本即可，虽然他这里给的unit是三个副本的数据，但是每个副本的数据都是一样的，所以我用第一个就ok了
  // 遍历这个对象的对象块
  // 如果当前磁头预消耗tokens小于当前磁头剩余tokens数量，一个if的行为对应读一个对象的对象块
  for (int i = 0; i < repDisk.size(); i++) {
    int diskID = repDisk[i];
    int checkHowManyTokensCost =
        (diskList[diskID - 1].howManyTokensCost(objUnit[0], whoever));
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
          isDone.second = j;
          // 如果读失败了，确实可以直接在这里直接修改obj的unitid和size用于下一个时间片接着读
          // 这里的set需要set一个vector类型的unit，对应于三个磁盘的起始unit
          for (int m{0}; m < repDisk.size(); m++) {
            setObjUnit[m] = objUnit[m] + j;
          }
          // 这里不能直接修改obj，因为如果同时有其他请求要读这个对象的话，那么其他请求就永远读不成功了
          //  obj.setObjectUnit(setObjUnit);
          //  obj.setObjectSize(objSize - j);
          break;
        }
      }
      if (judge == false) {
        // 如果没读到（一定是j才没读到），就返回这个磁盘的id,这里的readFailureForToken是暂时还没有用的，但是也是可以的
        readFailureForToken = diskID - 1;
      } else {
        isDone.first = true;
        completeRequest.push_back(readRequest.getRequestId());
      }
      break;
    }
  }

  return isDone;
}

pair<bool, int> handlerread::handlerRequestfromScheduler(
    readRequest readRequest, vector<int> objUnitFromOnce, int objSizeFromOnce) {
  bool whoever;
  pair<bool, int> isDone;
  isDone.first = false;

  int costTokens;
  // int objUnit[REP_NUM];
  Object obj = object_list.getObject(readRequest.getObjectId());
  vector<int> objUnit = objUnitFromOnce;
  vector<int> repDisk = obj.getObjectDisk();
  int objSize = objSizeFromOnce;

  bool judge = true;
  vector<int> setObjUnit;
  setObjUnit.resize(3);
  // 我现在每一个副本写的位置都是一样的，每个副本写在不同的磁盘，每个副本在各自磁盘的位置相同，所以我现在读，只需要读一个副本即可，虽然他这里给的unit是三个副本的数据，但是每个副本的数据都是一样的，所以我用第一个就ok了
  // 遍历这个对象的对象块
  // 如果当前磁头预消耗tokens小于当前磁头剩余tokens数量，一个if的行为对应读一个对象的对象块
  for (int i = 0; i < repDisk.size(); i++) {
    int diskID = repDisk[i];
    int checkHowManyTokensCost =
        (diskList[diskID - 1].howManyTokensCost(objUnit[0], whoever));
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
          isDone.second = j;
          // 如果读失败了，确实可以直接在这里直接修改obj的unitid和size用于下一个时间片接着读
          // 这里的set需要set一个vector类型的unit，对应于三个磁盘的起始unit
          // for (int m{0}; m < repDisk.size(); m++) {
          //   setObjUnit[m] = objUnit[m] + j;
          // }
          // 这里不能直接修改obj，因为如果同时有其他请求要读这个对象的话，那么其他请求就永远读不成功了
          //  obj.setObjectUnit(setObjUnit);
          //  obj.setObjectSize(objSize - j);
          break;
        }
      }
      if (judge == false) {
        // 如果没读到（一定是j才没读到），就返回这个磁盘的id,这里的readFailureForToken是暂时还没有用的，但是也是可以的
        readFailureForToken = diskID - 1;
      } else {
        isDone.first = true;
        completeRequest.push_back(readRequest.getRequestId());
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

#include "scheduler.h"
#include "globalValue.h"
#include "handlerDelete.h"
#include "handlerRead.h"
#include "handlerWrite.h"
// #include "object.h"
// #include "objectList.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <tuple>
#include <vector>

handlerdelete handleDelete;
handlerwrite handlerWrite;
handlerread handlerRead;

vector<Disk> diskList;

void Scheduler::myScheduler() {

  // V1版本删操作，将具体功能实现解耦到handler，scheduler只做指令的调度选择和分发
  vector<delRequest> delRequestList = del_request_list.getdelRequest();
  delRequest delRequest;
  if (delRequestList.size()) {
    for (int currentPos = 0; currentPos < delRequestList.size(); currentPos++) {
      handleDelete.handlerRequestfromScheduler(delRequestList[currentPos]);
    }
  } else {
    printf("0\n");
  }

  // V1版本写操作，将具体功能实现解耦到handler，scheduler只做指令的调度选择和分发
  vector<writeRequest> writeRequestList = write_request_list.getrWriteRequest();
  writeRequest writeRequest;
  for (int currentPos = 0; currentPos < writeRequestList.size(); currentPos++) {
    handlerWrite.handlerRequestfromScheduler(writeRequestList[currentPos]);
    handlerWrite.printCompleteRequest();
  }
  fflush(stdout);

  // V1版本读操作，将具体功能实现解耦到handler，scheduler只做指令的调度选择和分发
  vector<readRequest> readRequestList = read_request_list.getReadRequest();
  readRequest readRequest;
  if (readRequestList.size()) {
    for (int currentPos = 0; currentPos < readRequestList.size();
         currentPos++) {
      handlerRead.handlerRequestfromScheduler(readRequestList[currentPos]);
    }
    handlerRead.printCompleteRequest();
  } else {
    printf("0\n");
  }

  fflush(stdout);
}
// 一起接收请求并调度分发
//------------------------------------------------------------------------------------------------------
// 分开接收请求并调度分发
void Scheduler::myDeleteScheduler() {
  vector<delRequest> delRequestList = del_request_list.getdelRequest();
  delRequest delRequest;
  vector<int> deleteReadRequest;
  if (delRequestList.size()) {
    for (int currentPos = 0; currentPos < delRequestList.size(); currentPos++) {
      handleDelete.handlerRequestfromScheduler(delRequestList[currentPos]);
    }
    // 先去根据删除请求得到objId，然后再去readNotDone里面根据readRequestId找objId，然后对比objId如果相等则记录次数和readRequestId，循环完成之后进行输出，如果找到了，则也要将readNotDone里面的对应元素删除
    for (int i{0}; i < delRequestList.size(); i++) {
      // 根据删除请求得到了objId
      delRequestList[i].getObjectId();
      // 来遍历readNotDone
      for (auto it = readNotDone.begin(); it != readNotDone.end();) {
        // 根据readNotDone里面的readRequestId获得对应的objId，然后与之前获得的objId对比
        /*
        std::get<0>(*it)是三元组里面的第一个，也就是readRequest
        然后用read_request_list.getreadRequestByRequestId（）根据readRequest获得request对象
        然后再对这个对象使用getObjectId()获得objId再与用delRequest获得的objId做比较
        如果相等则记录并且在readNotDone里面删除
         */
        if (read_request_list.getreadRequestByRequestId(std::get<0>(*it))
                .getObjectId() == delRequestList[i].getObjectId()) {
          deleteReadRequest.push_back(std::get<0>(*it));
          it = readNotDone.erase(it);
        } else {
          ++it;
        }
      }
    }
    printf("%d\n", deleteReadRequest.size());
    for (int k{0}; k < deleteReadRequest.size(); ++k) {
      printf("%d\n", deleteReadRequest[k]);
    }
    fflush(stdout);
  } else {
    printf("0\n");
    fflush(stdout);
  }
}

void Scheduler::myWriteScheduler() {
  vector<writeRequest> writeRequestList = write_request_list.getrWriteRequest();
  writeRequest writeRequest;
  for (int currentPos = 0; currentPos < writeRequestList.size(); currentPos++) {
    handlerWrite.handlerRequestfromScheduler(writeRequestList[currentPos]);
    handlerWrite.printCompleteRequest();
  }
  fflush(stdout);
}

void Scheduler::myReadScheduler() {
  int flagggg = -1;
  handlerread handlerRead;
  vector<readRequest> readRequestList = read_request_list.getReadRequest();
  readRequest doLastRead;
  vector<int> readFailUnit;
  readFailUnit.resize(3);

  // 用于判断当前读请求是否完成
  pair<bool, int> isdone;
  int readFailId, readFailObjSize;
  // 对之前时间片中未读完的读请求做读操作
  if (readNotDone.size()) {
    for (auto it = readNotDone.begin(); it != readNotDone.end();) {
      // auto &element = readNotDone.back();

      int requestId = std::get<0>(*it);
      vector<int> &requestObjUnit = std::get<1>(*it);
      int &requestObjSize = std::get<2>(*it);
      isdone = handlerRead.handlerRequestfromScheduler(
          read_request_list.getreadRequestByRequestId(requestId),
          requestObjUnit, requestObjSize);
      if (isdone.first) {
        // readNotDone.pop_back();
        it = readNotDone.erase(it);
      } else {
        for (int n{0}; n < 3; n++) {
          requestObjUnit[n] += isdone.second;
          requestObjSize -= isdone.second;
        }
        ++it;
      }
    }
  }
  // 读了一个之后，在下一个时间片中又读这个，单磁头往前移动了，且磁头只能单向移动，此时可以选择读其他副本显然更优
  if (readRequestList.size()) {
    // 对当前时间片来的读请求做读操作
    for (int currentPos = 0; currentPos < readRequestList.size();
         currentPos++) {
      isdone =
          handlerRead.handlerRequestfromScheduler(readRequestList[currentPos]);
      if (!isdone.first) {
        // 获得读失败的当前请求Id
        readFailId = readRequestList[currentPos].getRequestId();
        // 将没有读完的读请求id放入readNotDone这个list中,因为我已经在handler的时候就修改了obj的unit和size,obj不能改
        // 获得读失败，下一个读的起始unit
        Object obj =
            object_list.getObject(readRequestList[currentPos].getObjectId());
        vector<int> objUnit = obj.getObjectUnit();
        int objSize = obj.getObjectSize();
        for (int m{0}; m < 3; m++) {
          readFailUnit[m] = objUnit[m] + isdone.second;
        }
        readFailObjSize = objSize - isdone.second;
        readNotDone.push_back(
            make_tuple(readFailId, readFailUnit, readFailObjSize));
      }
    }

    //  当j的时候不输出#
    for (int i{0}; i < maxDisk; i++) {
      diskList[i].printOncetimeDiskHeadAction();
      for (int q{0}; q < handlerRead.whichDiskIJumped.size(); q++) {
        if (handlerRead.whichDiskIJumped[q] == i) {
          flagggg = i;
          break;
        }
      }
      if (i != flagggg) {
        printf("#\n");
      }
      diskList[i].diskPrintCacheClear();
      diskList[i].diskDiskHeadInit();
    }
    handlerRead.printCompleteRequest();
  } else {
    for (int i{0}; i < maxDisk; i++) {
      diskList[i].printOncetimeDiskHeadAction();
      for (int q{0}; q < handlerRead.whichDiskIJumped.size(); q++) {
        if (handlerRead.whichDiskIJumped[q] == i) {
          flagggg = i;
          break;
        }
      }
      if (i != flagggg) {
        printf("#\n");
      }
      diskList[i].diskPrintCacheClear();
      diskList[i].diskDiskHeadInit();
    }
    handlerRead.printCompleteRequest();
  }

  fflush(stdout);
}
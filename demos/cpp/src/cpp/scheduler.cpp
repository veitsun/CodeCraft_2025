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
  if (delRequestList.size()) {
    for (int currentPos = 0; currentPos < delRequestList.size(); currentPos++) {
      handleDelete.handlerRequestfromScheduler(delRequestList[currentPos]);
    }
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
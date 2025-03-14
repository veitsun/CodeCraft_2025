#include "scheduler.h"
#include "globalValue.h"
#include "object.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
vector<Disk> diskList;

HandlerDelete delHandler;
writeHeadler writeHandler;
readHeadler readHandler;

void Scheduler::myScheduler() {

  Object obj;

  delRequest *delquest = del_request_list.getList();
  if (delquest) {
    // 调用handlerdelete

  } else {
    printf(0);
  }

  writeRequest *writequest = write_request_list.getList();
  if (writequest) {
    for (int i = 0; i < maxDisk; i++) {
      // 调用handlerwrite,没有写入的话,选手不需要有任何输出
      diskList[i].wherecanput(obj.getObjectTag());
    }
  }

  readRequest *readrequest = read_request_list.getList();
  if (readrequest) {
    // 调用handlerread
  } else {
    for (/* 磁盘个数 */;;) {
      printf("#\n");
    }
    printf(0);
  }
}
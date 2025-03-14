#include "scheduler.h"
#include "globalValue.h"
#include "object.h"
#include "objectList.h"
#include <cstdio>
#include <cstdlib>
#include <vector>

vector<Disk> diskList;

handlerdelete handleDelete;
handlerwrite handlerWrite;
handlerread handlerRead;

void Scheduler::myScheduler() {

  int k = 0;
  vector<pair<int, int>> section;

  vector<delRequest *> delquest = del_request_list.getList();
  int size = delquest.size();
  if (size) {
    while (size) {
      delquest[size]->getObjectId();

      // 占位

      // 调用handlerdelete

      size--;
    }

  } else {
    printf(0);
  }

  vector<writeRequest *> writeRequest = write_request_list.getList();
  int flag = 0;
  size = writeRequest.size();
  while (size) {
    vector<int> diskNum;
    vector<int> unit;
    diskNum.resize(REP_NUM);
    unit.resize(REP_NUM);
    for (int rep = 0; rep < REP_NUM; rep++) {
      // 找所有磁盘
      flag = 0;
      for (int i = 0; i < maxDisk; i++) {
        if (flag >= 1)
          break;
        // 问磁盘能放哪里
        section = diskList[i].wherecanput(writeRequest[size]->getObjectTag());
        // 现在只考虑了返回的一个vectorpair中一定是存在放得下的pair
        for (int j = 0; j < section.size(); j++) {
          if ((section[j].second - section[j].first) <
              writeRequest[size]->getObjectSize()) {
            // 调用handler写
            handlerWrite.handlerWrite2Disk(section[j].first,
                                           writeRequest[size]->getObjectId(),
                                           writeRequest[size]->getObjectSize());
            diskNum[rep] = i;
            unit[rep] = section[j].first;
            flag++;
            break;
          }
        }
      }
    }
    size--;
    object_list.setList(
        writeRequest[size]->getObjectId(), writeRequest[size]->getObjectTag(),
        writeRequest[size]->getObjectSize(), diskNum, unit);
  }

  vector<readRequest *> readrequest = read_request_list.getList();
  size = readrequest.size();
  if (size) {
    while (size) {
      size--;
    }
    // 调用handlerread
  } else {
    for (int i = 0; i < maxDisk; i++) {
      printf("#\n");
    }
    printf(0);
  }
}
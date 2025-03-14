#include "scheduler.h"
#include "globalValue.h"
#include "object.h"
#include "objectList.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

vector<Disk> diskList;

handlerdelete handleDelete;
handlerwrite handlerWrite;
handlerread handlerRead;

void Scheduler::myScheduler() {

  int diskUnique = 0;

  int k = 0;
  vector<pair<int, int>> section;

  vector<delRequest *> delrequest = del_request_list.getList();
  int size = delrequest.size();

  if (size) {
    while (size) {
      int objUnit[REP_NUM];
      int objSize;
      memcpy(objUnit,
             object_list.getObjectUnit(delrequest[size]->getObjectId()),
             sizeof(int) * REP_NUM);
      objSize = object_list.getObjectSize(delrequest[size]->getObjectId());

      for (int i = 0; i < REP_NUM; i++) {
        handleDelete.handlerDelete2Disk(objUnit[i], objSize);
      }
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
      for (int i = diskUnique; i < maxDisk; i++) {
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
            printf("%d\n", writeRequest[size]->getObjectId());
            for (int i = 0; i < writeRequest[size]->getObjectSize() + 1; i++) {
              printf("%d", object_list.getObjectDisk(
                               writeRequest[size]->getObjectId()));
              int start_pos =
                  *object_list.getObjectUnit(writeRequest[size]->getObjectId());
              for (int size = 0; size < writeRequest[size]->getObjectSize();
                   size++) {
                printf(" %d", start_pos + size);
              }
            }
            diskNum[rep] = i;
            unit[rep] = section[j].first;
            flag++;
            diskUnique++;
            break;
          }
        }
      }
    }
    size--;
    object_list.setList(
        writeRequest[size]->getObjectId(), writeRequest[size]->getObjectTag(),
        writeRequest[size]->getObjectSize(), diskNum.data(), unit.data());
  }

  // 读操作，现在的写操作和删除操作都是三个副本同步的,并且每一个副本的对象都是放在不同的磁盘上的（diskUnique）
  vector<readRequest *> readrequest = read_request_list.getList();
  size = readrequest.size();
  // 磁头的位置和剩余token量

  vector<Disk> disk;
  int objDisk[maxDisk];
  int objUnit[maxDiskSize];
  // pair<int, int> diskHeadInfo = disk.getDiskHead();
  int riskHeadposition[maxDisk][maxDiskSize];
  if (size) {
    while (size) {

      readrequest[size]->getObjectId();
      // 获得obj的磁盘号码
      // obj在磁盘的某些unit上面
      // object_list.getObjectUnit(readrequest[size]->getObjectId());

      memcpy(objDisk,
             object_list.getObjectDisk(readrequest[size]->getObjectId()),
             sizeof(int) * maxDisk);

      memcpy(objUnit,
             object_list.getObjectUnit(readrequest[size]->getObjectId()),
             sizeof(int) * maxDiskSize);
      // 根据objid来获取这个obj的起始unit
      // object_list.getObjectUnit(readrequest[size]->getObjectId());
      // object_list.getObjectSize(readrequest[size]->getObjectId());

      // 获得一个int *
      pair<int, int> diskHeadInfo = disk[objDisk[0]].getDiskHead();

      if (objUnit[0] - diskHeadInfo.first > maxToken) {
        handlerRead.handlerRead2Disk(3, objUnit[0]);
      } else if (objUnit[0] - diskHeadInfo.first == maxToken) {
        handlerRead.handlerRead2Disk(1, 1);
      } else {
        for (int i = 0; i < objUnit[0] - diskHeadInfo.first; i++) {
          handlerRead.handlerRead2Disk(2, 2);
        }
      }

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
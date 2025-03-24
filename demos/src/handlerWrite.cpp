#include "handlerWrite.h"
#include "disk.h"
#include "globalValue.h"
#include "object.h"
#include "objectList.h"
#include <cstdio>
#include <utility>
#include <vector>

handlerwrite handlerwrite;
int handlerwrite::static_read_diskid = 0;
/*
---------------------------BUG todo---------------------------
********若选手无法选出三块有足够空间的硬盘存放该对象则选手程序直接判********
该对象第二个副本存在最后一个磁盘，则最后一个副本放不下
---------------------------BUG todo---------------------------
 */
bool handlerwrite::handlerRequestfromScheduler(writeRequest writeRequest) {
  bool isDone = false;
  // int diskUnique = 0; // 控制每个rep写到不同磁盘
  tuple<bool, int, int> section;
  vector<int> diskNum;
  vector<int> unit;
  diskNum.resize(REP_NUM);
  unit.resize(REP_NUM);
  int flag = 0;

  // 这里做副本都写
  for (int rep{0}; rep < REP_NUM; rep++) {
    // printf("副本 %d 写入\n", rep);
    // fflush(stdout);
    flag = 0;
    for (int i_rep{static_read_diskid}; i_rep < static_read_diskid + maxDisk;
         i_rep++) {
      int i = i_rep;
      if (i_rep >= maxDisk) {
        i = i_rep % maxDisk;
      }
      if (flag >= 1)
        break;
      section = diskList[i].wherecanput(writeRequest.getObjectTag(),
                                        writeRequest.getObjectSize());
      // 假设返回的section存在可以放的
      {
        bool shifouyoukongjianfang = get<0>(section);
        int start = get<1>(section);
        int end = get<2>(section);
        if (shifouyoukongjianfang == true) {
          // 这里的判断是如果能放下的话就调用handler来放，但是现在V1，感觉这里可以直接调用disk
          diskList[i].diskWrite(start, writeRequest.getObjectSize());
          diskNum[rep] = i + 1;
          unit[rep] = start;
          flag++;
          static_read_diskid = i + 1;
          isDone = true;
          completeObjId = writeRequest.getObjectId();
          completeRep[rep] = i + 1;
          for (int unitId{start};
               unitId <= (start + writeRequest.getObjectSize() - 1); unitId++) {
            completeUnitId[rep].emplace_back(unitId);
          }
          break;
        }
      }
    }
  }

  // Object obj = object_list.getObject(writeRequest.getObjectId());
  Object obj;
  obj.setObjectId(writeRequest.getObjectId());
  obj.setObjectSize(writeRequest.getObjectSize());
  obj.setObjectTag(writeRequest.getObjectTag());
  obj.setObjectDisk(diskNum);
  obj.setObjectUnit(unit);
  object_list.addObject(obj);

  return isDone;
}

void handlerwrite::printCompleteRequest() {
  printf("%d\n", completeObjId);
  for (int i{0}; i < REP_NUM; i++) {
    printf("%d", completeRep[i]);
    for (int j{0}; j < completeUnitId[i].size(); j++) {
      printf(" %d", completeUnitId[i][j]);
    }
    printf("\n");
  }
  for (int i{0}; i < completeUnitId.size(); i++) {
    completeUnitId[i].clear();
  }
  // completeUnitId.clear();
}
#include "handlerWrite.h"
#include "disk.h"
#include "globalValue.h"
#include "object.h"
#include "objectList.h"
#include <cstdio>
#include <utility>
#include <vector>

handlerwrite handlerwrite;

/*
---------------------------BUG todo---------------------------
********若选手无法选出三块有足够空间的硬盘存放该对象则选手程序直接判********
该对象第二个副本存在最后一个磁盘，则最后一个副本放不下
---------------------------BUG todo---------------------------
 */
bool handlerwrite::handlerRequestfromScheduler(writeRequest writeRequest) {
  bool isDone = false;
  int diskUnique = 0; // 控制每个rep写到不同磁盘
  vector<pair<int, int>> section;
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
    for (int i{diskUnique}; i < maxDisk; i++) {
      if (flag >= 1)
        break;
      section = diskList[i].wherecanput(writeRequest.getObjectTag());
      // 假设返回的section存在可以放的
      for (int j{0}; j < section.size(); j++) {
        // printf("%d+++++%d\n", section[j].second - section[j].first,
        //        writeRequest.getObjectSize());
        if ((section[j].second - section[j].first) >=
            writeRequest.getObjectSize()) {
          // 这里的判断是如果能放下的话就调用handler来放，但是现在V1，感觉这里可以直接调用disk
          diskList[i].diskWrite(section[j].first, writeRequest.getObjectId(),
                                writeRequest.getObjectSize());
          diskNum[rep] = i + 1;
          unit[rep] = section[j].first;
          flag++;
          // diskUnique++;
          diskUnique = i + 1;
          isDone = true;
          completeObjId = writeRequest.getObjectId();
          completeRep[rep] = i + 1;
          // for (int unitId{section[j].first}; i <
          // writeRequest.getObjectSize();
          for (int unitId{section[j].first};
               unitId <= (section[j].first + writeRequest.getObjectSize() - 1);
               unitId++) {
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
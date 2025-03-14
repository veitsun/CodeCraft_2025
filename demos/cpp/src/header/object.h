#pragma once

#define MAX_DISK_NUM (10 + 1)
#define MAX_DISK_SIZE (16384 + 1)
#define MAX_REQUEST_NUM (30000000 + 1)
#define MAX_OBJECT_NUM (100000 + 1)
#define REP_NUM (3)
#define FRE_PER_SLICING (1800)
#define EXTRA_TIME (105)

// #include "globalDefines.h"
#include "globalDefines.h"
#include "objectList.h"


//从指令中获取写指令的信息
class Object {
private:
  int object_id;            // 对象id
  int tag_id;               // 对象所属的tag
  int object_size;          // 对象大小
  int disk_num[REP_NUM + 1]; // 存放副本的磁盘编号
  int unit[REP_NUM + 1];    // 每个副本的所在磁盘上的存储单元开始编号
public:
  Object() : object_id(0), tag_id(0), object_size(0), disk_num{0}, unit{0} {}
  void setObject(int object_id, int tag_id, int object_size, int* disk_num, int* unit){}

  int getObjectId(){return object_id;}
  int getObjectTag(){return tag_id;}
  int getObjectSize(){return object_size;}
  int* getObjectDisk(){return disk_num;}
  int* getObjectUnit(){return unit;}

};
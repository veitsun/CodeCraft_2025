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
// #include "objectList.h"
#include <vector>

using namespace std;

// 从指令中获取写指令的信息
class Object {
private:
  int object_id;             // 对象id
  int object_tag;                // 对象tag
  int object_size;           // 对象大小
  vector<int> object_disk; // 存放副本的磁盘编号
  vector<int> object_unit;     // 每个副本的所在磁盘上的存储单元开始编号
public:
  Object(){
    object_id = 0;
    object_tag = 0;
    object_size = 0;
    object_disk = vector<int>(REP_NUM, 0);
    object_unit = vector<int>(REP_NUM, 0);
  }
  void setObjectId(int id) {object_id = id;}
  void setObjectTag(int tag) {object_tag = tag;}
  void setObjectSize(int size) {object_size = size;}
  void setObjectDisk(vector<int> disk) {object_disk = disk;}
  void setObjectUnit(vector<int> unit) {object_unit = unit;}
  int getObjectId() { return object_id; }
  int getObjectTag() { return object_tag; }
  int getObjectSize() { return object_size; }
  vector<int> getObjectDisk() { return object_disk; }
  vector<int> getObjectUnit() { return object_unit; }
};
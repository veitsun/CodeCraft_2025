#pragma once

#include "globalDefines.h"
class Object {
private:
  int object_id;            // 对象id
  int tad_id;               // 对象所属的tag的id
  int replica[REP_NUM + 1]; // 存放副本的磁盘编号
  int *unit[REP_NUM + 1];   // 每个副本的所在磁盘上的存储单元编号
  int size;                 // 对象大小
  int is_deleted;           // 是否被删除
public:
  Object() : object_id(0), tad_id(0), size(0), is_deleted(0) {
    for (int i = 0; i < REP_NUM + 1; i++) {
      replica[i] = 0;
      unit[i] = nullptr;
    }
  }

  Object(int object_id, int tad_id, int size)
      : object_id(object_id), tad_id(tad_id), size(size), is_deleted(0) {
    for (int i = 0; i < REP_NUM + 1; i++) {
      replica[i] = 0;
      unit[i] = nullptr;
    }
  }

  // Getters and setters
  int getObjectId() const { return object_id; }
  void setObjectId(int id) { object_id = id; }

  int getTadId() const { return tad_id; }
  void setTadId(int id) { tad_id = id; }

  int getSize() const { return size; }
  void setSize(int size) { this->size = size; }

  int getIsDeleted() const { return is_deleted; }
  void setIsDeleted(int is_deleted) { this->is_deleted = is_deleted; }

  int getReplica(int index) const { return replica[index]; }
  void setReplica(int index, int disk_id) { replica[index] = disk_id; }

  int *getUnit(int index) const { return unit[index]; }
  void setUnit(int index, int *unit) { this->unit[index] = unit; }
};
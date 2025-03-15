/**
    保存磁盘每个块的信息
    写功能 wirte(int position, diskUnit disk_unit)
    参数：存储到磁盘的哪个位置，存储单元 diskunit 读功能
    参数：读磁盘的哪个位置，返回存储单元diskunit 删除功能 参数：删除磁盘的位置
*/
#pragma once

#include <tuple>
#define MAX_DISK_NUM (10 + 1)
#define MAX_DISK_SIZE (16384 + 1)
#define MAX_REQUEST_NUM (30000000 + 1)
#define MAX_OBJECT_NUM (100000 + 1)
#define REP_NUM (3)
#define FRE_PER_SLICING (1800)
#define EXTRA_TIME (105)

#include "diskPointer.h"
#include "diskUnit.h"
#include <cstdio>
#include <vector>

using namespace std;

class Disk {
private:
  // 这里应该是一开始输入 V 的大小，每个磁盘的存储单元数量是固定V个的
  std::vector<DiskUnit> storage; // storage 的大小是V
  DiskPointer pointer;
  // vector<pair<int, int>> res;
  void reset();
  int max(int a, int b) { return a > b ? a : b; }
  int min(int a, int b) { return a < b ? a : b; }

public:
  Disk() { storage.resize(MAX_DISK_SIZE); } // 这个之后改成 V 大小
  ~Disk() {}

  // 最新版本是我不需要提供磁盘块号，我只是单个磁盘的抽象
  DiskUnit getStorageUnit(
      int Unitindex) const; // 传入存储单元编号，返回磁盘单元中的数据对象
  void setStorageUnit(
      int Unitindex,
      DiskUnit unit); // 传入存储单元编号，和要存入的对象。返回值为空.
                      // 写
  std::vector<DiskUnit> getDiskState(); // 返回磁盘块的磁盘单元数组

  void deleteStorageUnit(int Unitindex); // 删除磁盘块的磁盘单元数组中的数据

  void diskWrite(int unit_id, int obj_id, int obj_size); // 写功能

  void diskDelete(int unit_id, int obj_size); // 删除功能
  void diskRead(int action, int unit_id);     // 读功能

  void executeAction(int action, int unit_id, int obj_id, int obj_blockid);

  // void readValue(int unit_id); // 读功能
  void writeValueByTag(int unit_id, int obj_id, int obj_blockid, int tag_id);

  vector<pair<int, int>>
  getStorageUnitIntervalByTag(int tag_id) const; // 返回所有这个tag的区间

  vector<pair<int, int>> wherecanput(
      int tag_id) const; // 返回所有这个tag的区间, 并且这个区间都是空闲的

  // tuple<int tag_id, int start, int end
  vector<tuple<int, int, int>> getTagInterval(int tag) const;

  pair<int, int> getDiskHead();
  // int get
  // bool diskRead(int unit_id);

  int remainTokens();                             // 当前磁头剩余token数
  int howManyTokensCost(int objUnit, bool &path); // 预消耗token数

  bool diskRead(int unit_id); // 读单个对象块,执行具体的读动作,读取成功返回 true

  bool executeJump(int unit_id); // 需要执行jump操作,这是具体的执行动作

  int pathLen(int current_pos, int unit_id); // 这个还没所有实现
};

class NewDisk : public Disk {
private:
  /* data */

  std::vector<DiskUnit> storage; // storage 的大小是V
  DiskPointer pointer;

public:
  NewDisk() { storage.resize(MAX_DISK_SIZE); } // 这个之后改成 V 大小
  ~NewDisk() {}

  vector<pair<int, int>>
  getStorageUnitIntervalByTag(int tag_id) const; // 返回所有这个tag的区间
};
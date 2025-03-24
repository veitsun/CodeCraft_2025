/**
    保存磁盘每个块的信息
    写功能 wirte(int position, diskUnit disk_unit)
    参数：存储到磁盘的哪个位置，存储单元 diskunit 读功能
    参数：读磁盘的哪个位置，返回存储单元diskunit 删除功能 参数：删除磁盘的位置
*/
#pragma once

#include <tuple>

#include "SonDisk.h"
#include "diskPointer.h"
#include "diskUnit.h"
#include <cstdio>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class Disk {
private:
  // 这里应该是一开始输入 V 的大小，每个磁盘的存储单元数量是固定V个的
  std::vector<DiskUnit> storage; // storage 的大小是V
  DiskPointer pointer;
  // vector<pair<int, int>> res;
  // void reset(); // 这个暂且没有用
  int max(int a, int b) { return a > b ? a : b; }
  int min(int a, int b) { return a < b ? a : b; }
  void setTagDistribute(); // 这个已经升级到 v2 版本
  vector<int> RCOS{64,  116, 158, 192, 220,
                   243, 262, 278, 294 /*, 310, 326, 342, 358*/};
  string cache{};
  // vector<char> cache;
  // 第一个 int 是开始区间，第二个int 是结束区间
  vector<pair<int, int>> tag_interval; // 16 个tag的区间
  int func_add_rcos(int distance, bool pre_isornot_read);
  int freeUnitSize;
  int freeMaxTagIntervalSize;
  int freeMaxTagIntervalStart = 0;
  int freeMaxTagIntervalEnd = 0;

public:
  Disk(); // 这个之后改成 V 大小
  ~Disk() {}
  vector<vector<pair<int, int>>> free_interval; // 每个tag 的空闲空间
  bool isNotNum_func(char c);
  int getfreeUnitSize() const { return freeUnitSize; }
  tuple<int, int, int> getfreeMaxTagIntervalSize();

  // void addObjectInDisk();
  void setTagDistribute_v2();
  void diskPrintCacheClear();
  void diskDiskHeadInit();
  int getHeadPosition() const { return pointer.current_position; }

  // 最新版本是我不需要提供磁盘块号，我只是单个磁盘的抽象
  void getOncetimeDiskHeadAction();

  void diskWrite(int unit_id, int obj_size);             // 写功能
  void diskWrite(int unit_id, int obj_size, int tag_id); // 写功能

  void diskDelete(int unit_id, int obj_size);             // 删除功能
  void diskDelete(int unit_id, int obj_size, int tag_id); // 删除功能

  // 返回这个磁盘有没有空闲空间存，空闲空间起始位置 结束位置
  tuple<bool, int, int> wherecanput(int tag_id, int obj_size);

  // tuple<int tag_id, int start, int end
  vector<tuple<int, int, int>> getTagInterval(int tag) const;

  int remainTokens();                             // 当前磁头剩余token数
  int howManyTokensCost(int objUnit, bool &path); // 预消耗token数

  bool diskRead(int unit_id); // 读单个对象块,执行具体的读动作,读取成功返回 true

  int pathLen(int current_pos, int unit_id); // 判断长度

  bool executeJump(int unit_id); // 需要执行jump操作,这是具体的执行动作

  int realPosition(int unit_relative_id); // 因为是循环的存储单元队列
};

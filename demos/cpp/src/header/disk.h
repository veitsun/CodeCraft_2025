/**
    保存磁盘每个块的信息
    写功能 wirte(int position, diskUnit disk_unit)
    参数：存储到磁盘的哪个位置，存储单元 diskunit 读功能
    参数：读磁盘的哪个位置，返回存储单元diskunit 删除功能 参数：删除磁盘的位置
*/
#pragma once

#include "diskPointer.h"
#include "diskUnit.h"
#include "globalDefines.h"
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

class Disk {
private:
  // 这里应该是一开是输入 V 的大小，每个磁盘的存储单元数量是固定V个的
  std::vector<void *> storage[MAX_DISK_SIZE];
  DiskPointer pointer;
  // std::vector<int> storage[MAX_DISK_SIZE];
  void reset();

public:
  Disk() {}
  ~Disk() {}

  // 最新版本是我不需要提供磁盘块号，我只是单个磁盘的抽象
  // int getDiskId() const;  // 返回磁盘块号
  // void setDiskId(int id); // 修改磁盘对象所在的块号
  // std::vector<void *> getStorageValue(
  //     int Unitindex) const; // 传入存储单元编号，返回磁盘单元中的数据对象
  void
  setStorageValue(int Unitindex,
                  const std::vector<void *>
                      &value); // 传入存储单元编号，和要存入的对象。返回值为空
  std::vector<void *> *getStorageArray(); // 返回磁盘块的磁盘单元数组首地址

  void deleteStorageValue(int Unitindex); // 删除磁盘块的磁盘单元数组中的数据

  void writeValue(int unit_id, int obj_id, int obj_blockid); // 写功能

  void deleteValue(int unit_id);           // 删除功能
  void readValue(int action, int unit_id); // 读功能

  void executeAction(int action, int unit_id, int obj_id, int obj_blockid);

  // void readValue(int unit_id); // 读功能
};
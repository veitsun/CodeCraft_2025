/**
    保存磁盘每个块的信息
    写功能 wirte(int position, diskUnit disk_unit)
    参数：存储到磁盘的哪个位置，存储单元 diskunit 读功能
    参数：读磁盘的哪个位置，返回存储单元diskunit 删除功能 参数：删除磁盘的位置
*/
#pragma once

#include "diskUnit.h"
#include "globalDefines.h"
#include <cstdio>
#include <vector>

using namespace std;

class Disk {
private:
  int diskId; // 磁盘块号
  // 该对象所对应磁盘块号的磁盘单元数组,磁盘单元里所存的数据结构还不确定
  std::vector<void *> storage[MAX_DISK_SIZE];
  void reset();

public:
  Disk() {};
  ~Disk() {}

  int getDiskId() const;  // 返回磁盘块号
  void setDiskId(int id); // 修改磁盘对象所在的块号
  std::vector<void *> getStorageValue(
      int Unitindex) const; // 传入存储单元编号，返回磁盘单元中的数据对象
  void
  setStorageValue(int Unitindex,
                  const std::vector<void *>
                      &value); // 传入存储单元编号，和要存入的对象。返回值为空
  std::vector<void *> *getStorageArray(); // 返回磁盘块的磁盘单元数组首地址
};
#pragma once

#include <vector>

using namespace std;

/*
    磁盘磁头类
        1. 保存现在的位置信息
        2. 对外暴露移动的操作
            1. 操作过程输出信息（"r"、"j"...）
            2. 操作过程中消耗的token
*/
class DiskPointer {
private:
public:
  DiskPointer(/* args */);
  ~DiskPointer();
};

DiskPointer::DiskPointer(/* args */) {}

DiskPointer::~DiskPointer() {}

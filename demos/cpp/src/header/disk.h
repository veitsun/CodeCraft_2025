#pragma once

#include <vector>

using namespace std;

/*
    磁盘类
        保存每个块的占用信息
        磁盘内存储的tag和为其预分配的空间信息
        引用磁头类
*/
class Disk {
private:
public:
  Disk(/* args */);
  ~Disk();
};

Disk::Disk(/* args */) {}

Disk::~Disk() {}

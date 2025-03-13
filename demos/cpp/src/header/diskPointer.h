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
  // int current_position;
  // int token;

public:
  int current_position;
  int token;
  DiskPointer(); // 在构造函数中初始化磁头位置和token，还未完成
  DiskPointer(int token);
  ~DiskPointer();
};

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
  int current_position; // 当前磁头的位置
  int token;            // 当前的token数
  bool pre_is_read;     // 上一个磁头的动作是不是read,若不是,则小号64t
  int pre_token;        // 上一次动作消耗的
                        // token,这个每个事件片都需要重新赋值为maxToken
  int read_nums;        // 当前磁头读了多少次
  DiskPointer();        // 在构造函数中初始化磁头位置和token，还未完成
  // DiskPointer(int token);
  ~DiskPointer();
};

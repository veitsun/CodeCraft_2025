#pragma once

#include "globalDefines.h"

#include <tuple>
#include <vector>

using namespace std;

#define ACITON_TYPE_COUNT (3)

/*
  <0, 0, 100> 代表 tag0 存储在当前磁盘的 0~100
*/
typedef tuple<int, int, int> TagDistribute;
/*
  对于一个磁盘，它的 tag 分配是这样的
  <0, 0, 100>
  <1, 100, 200>
  <2, 200, 300>
*/
typedef vector<tuple<int, int, int>> TagDistributeInDisk;

enum action_type { DELETE, WRITE, READ };

/*
  预处理模块
    读取全局信息
      构建每一个tag的全部大时间片的频率数组
      构建每段时间的全部tag的频率数组
    根据频率信息
      分配tag的备份磁盘
      分配tag的磁盘区域
*/
class PreProcess {
private:
  /*
    action_on_block_count[i][j][k]
      i： 代表操作类型 delete、write、read
      j： 代表标签类型
      k： 代表大时间片
  */
  vector<vector<vector<int>>> actionOnBlockCount;
  vector<int> maxSpaceForTag;
  // ! tagDistributeInAllDisk[1] 才是第一个磁盘
  vector<TagDistributeInDisk> tagDistributeInAllDisk;
  vector<tuple<int, int, int>> tagRepID;

public:
  PreProcess() {};
  ~PreProcess() {};
  int run();
};
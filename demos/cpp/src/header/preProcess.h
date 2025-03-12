#pragma once

#include <vector>

using namespace std;

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
  int maxTime, maxTag, maxDisk, maxDiskSize, maxToken;

  void AcceptGlobleParameter(void);

public:
  PreProcess();
};
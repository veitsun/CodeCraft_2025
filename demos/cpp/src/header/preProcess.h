#pragma once

#include <tuple>
#include <vector>

using namespace std;

#define ACITON_TYPE_COUNT (3)

enum action_type { DELETE, WRITE, READ };

/*
  <1, 0, 100> 代表 tag1 存储在当前磁盘的 0~100
*/
typedef tuple<int, int, int> TagDistribute;
/*
  对于一个磁盘，它的 tag 分配是这样的
  <0, 0, 100>
  <1, 100, 200>
  <2, 200, 300>
*/
typedef vector<tuple<int, int, int>> TagDistributeInDisk;

class PreProcess {
private:
  void acceptInput();
  void calculateTagSpace();
  void allockDiskGroupSpaceForTag(vector<int> &diskUsedSpace,
                                  vector<bool> &haveDistributeTag);
  void allockDiskSpaceForTag(vector<int> &diskUsedSpace,
                             vector<bool> &haveDistributeTag);

public:
  PreProcess() {};
  ~PreProcess() {};
  int run();
};
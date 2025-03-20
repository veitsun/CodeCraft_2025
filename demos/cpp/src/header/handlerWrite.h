#pragma once
#include "globalDefines.h"
#include "request.h"
#include <utility>
#include <vector>
using namespace std;
// class writeHeadler {
class handlerwrite {
private:
  int completeObjId;
  vector<int> completeRep;
  vector<vector<int>> completeUnitId;
  /* data */
public:
  static int static_read_diskid;
  // vector有一个构造函数可以接收一个整数参数，这个整数参数就表示容器的大小，同时将所有元素初始化为默认值，所以这里是相当于初始化completeRep和completeUnitId
  handlerwrite(/* args */) : completeRep(REP_NUM), completeUnitId(REP_NUM) {};
  ~handlerwrite() {};
  bool handlerRequestfromScheduler(writeRequest writeRequest);
  void handlerWrite2Disk(int unitStart, int objId, int objSize);
  void printCompleteRequest();
};

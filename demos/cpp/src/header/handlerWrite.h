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
  handlerwrite(/* args */) : completeRep(REP_NUM), completeUnitId(REP_NUM) {};
  ~handlerwrite() {};
  bool handlerRequestfromScheduler(writeRequest writeRequest);
  void handlerWrite2Disk(int unitStart, int objId, int objSize);
  void printCompleteRequest();
};
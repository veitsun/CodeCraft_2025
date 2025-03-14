#pragma once
#include <utility>
#include <vector>
using namespace std;
// class writeHeadler {
class handlerwrite {
private:
  /* data */
public:
  handlerwrite(/* args */) {};
  ~handlerwrite() {};
  void handlerWrite2Disk(int unitStart, int objId, int objSize);
};
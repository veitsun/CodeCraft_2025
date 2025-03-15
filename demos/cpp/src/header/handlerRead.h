#pragma once

#include "request.h"
class handlerread {
private:
  /* data */
  vector<int> completeRequest;

public:
  handlerread(/* args */) {};
  ~handlerread() {};
  void handlerRead2Disk(int action, int unitId);
  bool handlerRequestfromScheduler(readRequest readRequest);
  void printCompleteRequest();
};
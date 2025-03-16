#pragma once

#include "request.h"
class handlerdelete {
private:
  /* data */
public:
  handlerdelete(/* args */) {};
  ~handlerdelete() {};
  void handlerDelete2Disk(int unitId, int size);
  bool handlerRequestfromScheduler(delRequest delRequest);
  void printCompleteRequest();
};

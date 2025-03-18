#pragma once

#include "request.h"
#include <vector>
class handlerread {
private:
  /* data */
  vector<int> completeRequest;

public:
  int readFailureForJump = -1;
  int readFailureForToken = -1;
  vector<int> whichDiskIJumped;
  handlerread(/* args */) {

  };
  ~handlerread() {};
  void handlerRead2Disk(int action, int unitId);
  // 返回当前是否成功，如果没有成功的话，这里的int指的就是已经读了多少个unit
  // 添加两个参数objUnit和objSize针对于跨时间片进行读操作
  pair<bool, int> handlerRequestfromScheduler(readRequest readRequest);
  // 这个是对于上个没完成的读操作进行继续的读操作，传入的Unit和Size是上次没读完剩下的
  pair<bool, int> handlerRequestfromScheduler(readRequest readRequest,
                                              vector<int> objUnitFromOnce,
                                              int objSizeFromOnce);
  void printCompleteRequest();
};
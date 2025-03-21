#pragma once

#include "request.h"
#include <vector>
class handlerread {
private:
  /* data */
  vector<int> completeRequest;

public:
  int readFailureForJump = -1;
  vector<int> whichDiskIJumped;
  handlerread(/* args */) {};
  ~handlerread() {};
  void handlerRead2Disk(int action, int unitId);
  // 第一个参数是是否读成功，第二参数是读了多少个对象块，第三个参数是这次读的磁盘号，第四个参数是起始Unit位置,第五个参数是如果这个请求要做未做就true
  tuple<bool, int, int, int, bool>
  handlerRequestfromScheduler(readRequest readRequest);
  // 这个是对于上个没完成的读操作进行继续的读操作，传入的Unit和Size是上次没读完剩下的，默认objDiskId为-1
  pair<bool, int> handlerRequestfromScheduler(readRequest readRequest,
                                              int objUnitFromOnce,
                                              int objSizeFromOnce,
                                              int objDiskId = -1);
  void printCompleteRequest();
};
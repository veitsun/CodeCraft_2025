#pragma once
// #include "globalDefines.h"
// #include "handlerDelete.h"
// #include "handlerRead.h"
// #include "handlerWrite.h"
// #include "preProcess.h"

#include "request.h"
#include <cstdio>
#include <list>
#include <tuple>
#include <vector>
using namespace std;

class Scheduler {
private:
  /* data */
public:
  Scheduler(/* args */) {};
  ~Scheduler() {};
  // 没有完成的读请求vector，请求号、剩余unit起始位置、剩余unit数量,list是一个链表，需要再加一个参数存磁盘id
  // 这里存做而已做
  list<tuple<int, int, int, int>> readNotDone;
  list<tuple<int, int, int, int>> readNotDoneUnexpected;
  int readNotDoneUnexpectedCount = 0;
  // 这里存做而未做
  list<readRequest> readNotRead;

  void myScheduler();

  void myDeleteScheduler();
  void myWriteScheduler();
  void myReadScheduler();

  //...
};

#pragma once
// #include "globalDefines.h"
// #include "handlerDelete.h"
// #include "handlerRead.h"
// #include "handlerWrite.h"
// #include "preProcess.h"

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
  list<tuple<int, vector<int>, int, int>> readNotDone;
  void myScheduler();

  void myDeleteScheduler();
  void myWriteScheduler();
  void myReadScheduler();

  //...
};

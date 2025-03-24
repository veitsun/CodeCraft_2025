#pragma once

#include "globalDefines.h"
#include "request.h"
#include "requestDelList.h"
#include "requestReadList.h"
#include "requestWriteList.h"
#include <memory>
#include <vector>

using namespace std;

/*
  请求接受类
    根据请求的种类（Del、Read、Write）和请求对象的tag挂到三种待完成请求队列
*/

class requestReceiver {
private:
  int time_stamp;

public:
  requestReceiver() : time_stamp(0) {}
  int timestamp_action();
  void delRequestAdd();
  void writeRequestAdd();
  void readRequestAdd();
};
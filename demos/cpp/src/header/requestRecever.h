#pragma once

#include "request.h"
#include "delRequestList.h"
#include "readRequestList.h"
#include "writeRequestList.h"
#include <vector>

using namespace std;

/*
  请求接受类
    根据请求的种类（Del、Read、Write）和请求对象的tag挂到三种待完成请求队列
*/
class requestReceiver {
private:
  int time_stamp;
  int request_num;
public:
  requestReceiver() : time_stamp(0), request_num(-1) {}
  delRequestList* delListInit();
  writeRequestList* writeListInit();
  readRequestList* readListInit();
  void timestamp_action();
  void requestAdd(delRequestList* del_request_list, writeRequestList* write_request_list, readRequestList* read_request_list);
};
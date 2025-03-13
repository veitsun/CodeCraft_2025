#pragma once
#include "request.h"

#include <list>
#include <vector>

using namespace std;

/*
  待完成写请求队列类

  */
class writeRequestList {
private:
  list<writeRequest*> write_request_list;

public:
  writeRequestList(){};
  void addList(writeRequest* request);
  writeRequest* outList();
  bool listIsEmpty();
};
#pragma once
#include "request.h"

#include <list>
#include <vector>

using namespace std;

/*
  待完成删除请求队列类

  */
class delRequestList {
private:
  list<delRequest*> del_request_list;//待完成删除队列

public:
  delRequestList(){};
  void addList(delRequest* request);
  delRequest* outList();
  bool listIsEmpty();
};
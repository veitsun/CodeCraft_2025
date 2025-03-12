#pragma once

#include "request.h"

#include <list>
#include <vector>

using namespace std;

/*
  待完成读请求队列类

  */
class readRequestList {
private:
  list<Request> read_request_list;

public:
  readRequestList();
};
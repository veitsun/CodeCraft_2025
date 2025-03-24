#pragma once

#include "request.h"

#include <list>
#include <vector>

using namespace std;

/*
  待完成读请求队列类
  这边还存在优化，预想的是修改数据结构，把读请求按照tag类型进行分类，挂载在不同的队列上
  多加tag个指针，多加一个指向last_tag的int类型
  */
class readRequestList {
private:
  list<readRequest> readrequestlist;

public:
  readRequestList() {};
  void addReadRequest(readRequest request);
  vector<readRequest> getReadRequest();
  bool listIsEmpty();
  readRequest getreadRequestByRequestId(int requestId);
  void deletereadRequestByRequestId(int requestId);
  void clearReadRequestList() { readrequestlist.clear(); };
};
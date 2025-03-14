#pragma once

#include <list>
#include <vector>

using namespace std;

/*
  请求类
    参考demo

  */

// 父类如果需要使用则还需要修改！！！！！！
class Request {
private:
  int request_id; // 请求id
  int obj_id;     // 对象id
  int prev_id;    // 前一个请求id
  bool is_done;   // 是否已经完成
  int obj_size;   // 对象大小
  int obj_tag;    // 对象标签
  int request_time_stamp;
  // int obj_size;
  // int obj_blockid;
  // int action;
  // int disk_id;
  // int disk_blockid;
  // int disk_unitid;
  // int disk_position;
  // int token;
  // int extra_time;
  // int fre_per_slicing;
  // int rep_num;
  // int max_disk_num;
  // int max_disk_size;
  // int max_request_num;
  // int max_object_num;

public:
  // 初始化
  Request()
      : request_id(0), obj_id(0), prev_id(0), is_done(false), obj_size(0),
        obj_tag(0) {}

  Request(int request_id, int obj_id)
      : obj_id(obj_id), request_id(request_id), prev_id(0), is_done(false) {}

  // Getters and setters
  int getObjectId() const { return obj_id; }
  void setObjectId(int id) { obj_id = id; }

  int getRequestId() const { return request_id; }
  void setRequestId(int id) { request_id = id; }

  int getPrevId() const { return prev_id; }
  void setPrevId(int id) { prev_id = id; }

  bool getIsDone() const { return is_done; }
  void setIsDone(bool done) { is_done = done; }
  void freeRequest(Request *request) { delete (request); }
};

class delRequest : public Request {
private:
  int obj_id;
  int request_time_stamp;

public:
  delRequest() : request_time_stamp(0), obj_id(0) {}
  void setObjectId(int id) { obj_id = id; }
  int getObjectId() { return obj_id; }
  void setTime(int time) { request_time_stamp = time; }
  int getTime() { return request_time_stamp; }
  void freeDelRequest(delRequest *request) { delete (request); }
};

class writeRequest : public Request {
private:
  int obj_id;
  int obj_size;
  int obj_tag;
  int request_time_stamp;

public:
  writeRequest() : request_time_stamp(0), obj_id(0), obj_size(0), obj_tag(0) {}
  void setObjectId(int id) { obj_id = id; }
  int getObjectId() { return obj_id; }
  void setObjectSize(int size) { obj_size = size; }
  int getObjectSize() { return obj_size; }
  void setObjectTag(int tag) { obj_tag = tag; }
  int getObjectTag() { return obj_tag; }
  void setTime(int time) { request_time_stamp = time; }
  void freeWriteRequest(writeRequest *request) { delete (request); }
};

class readRequest : public Request {
private:
  int request_id;
  int obj_id;
  int request_time_stamp;
  bool is_done;

public:
  readRequest()
      : request_id(0), obj_id(0), request_time_stamp(0), is_done(false) {}
  void setRequestId(int id) { request_id = id; }
  int readRequestId() { return request_id; }
  void setObjectId(int id) { obj_id = id; }
  int getObjectId() { return obj_id; }
  void setTime(int Time) { request_time_stamp = Time; }
  int getTime() { return request_time_stamp; }
  void freeReadRequest(readRequest *request) { delete (request); }
};
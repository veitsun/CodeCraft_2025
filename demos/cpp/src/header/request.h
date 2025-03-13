#pragma once

#include <list>
#include <vector>

using namespace std;

/*
  请求类
    参考demo

  */
class Request {
private:
  int request_id; // 请求id
  int obj_id;     // 对象id
  int prev_id;    // 前一个请求id
  bool is_read;   // 是否是读请求
  bool is_done;   // 是否已经完成
  int obj_size;   // 对象大小
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
  Request() : request_id(0), obj_id(0), prev_id(0), is_done(false) {}

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
};
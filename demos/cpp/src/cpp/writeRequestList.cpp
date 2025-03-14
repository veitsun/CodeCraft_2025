#include "writeRequestList.h"

void writeRequestList::setList(writeRequest *request) {
  write_request_list.push_back(request); // 放在队尾
}

bool writeRequestList::listIsEmpty() { return write_request_list.empty(); }

vector<writeRequest*> writeRequestList::getList() {
  std::vector<writeRequest*> request;
  while(!writeRequestList::listIsEmpty()){
    // 弹出队头dd
    request.push_back(write_request_list.front());
    write_request_list.pop_front();
  }
  return request;
}
#include "readRequestList.h"
#include "globalValue.h"
#include "request.h"

void readRequestList::addReadRequest(readRequest request) {
  readrequestlist.push_back(request); // 放在队尾
}

bool readRequestList::listIsEmpty() { return readrequestlist.empty(); }

vector<readRequest> readRequestList::getReadRequest() {
  std::vector<readRequest> request;
  for (auto it = readrequestlist.begin(); it != readrequestlist.end(); ++it) {
    // 弹出队头dd
    if (it->getTime() == currentTime)
      request.push_back(*it);
  }
  return request;
}

readRequest readRequestList::getreadRequestByRequestId(int requestId) {
  for (auto it = readrequestlist.end(); it != readrequestlist.begin(); --it) {
    readRequest obj = *it;
    if (obj.getRequestId() == requestId)
      return obj;
  }
}

// #include "globalDefines.h"
#include "globalValue.h"
#include "handlerScheduler.h"
#include "preProcess.h"
#include "requestRecever.h"

int currentTime;
objectList object_list{};
vector<Disk> diskList;

int main() {
  // 预处理
  PreProcess preProcess;
  preProcess.run();
  diskList.resize(maxDisk);
  for (int i = 0; i < diskList.size(); i++) {
    diskList[i].setTagDistribute_v2();
  }

  // 请求接收器和三种请求队列
  requestReceiver request_receiver;
  // 调度器
  Scheduler scheduler;

  while (currentTime <= maxTime + 105) {
    // 同步时间戳
    currentTime = request_receiver.timestamp_action();
    // 接收删除请求
    request_receiver.delRequestAdd();
    // 处理删除请求
    scheduler.myDeleteScheduler();
    // 接受写入请求
    request_receiver.writeRequestAdd();
    // 处理写入请求
    scheduler.myWriteScheduler();
    // 先清空读请求队列，再读入新的请求
    // read_request_list.clearReadRequestList();
    request_receiver.readRequestAdd();
    // 处理读取请求
    scheduler.myReadScheduler();
  }

  return 0;
}
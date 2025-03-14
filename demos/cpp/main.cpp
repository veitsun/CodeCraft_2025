#include "globalDefines.h"
#include "globalValue.h"
#include "preProcess.h"
#include "requestRecever.h"
#include "scheduler.h"

int currentTime;
int main() {
  // 预处理
  PreProcess preProcess;
  preProcess.run();

  // 请求接收器和三种请求队列
  requestReceiver *request_receiver = new requestReceiver();

  // readRequestList = readRequestList.init();

  /*
    队列的初始化工作由自己提供
  */

  // del_request_list = request_receiver->delListInit();
  // write_request_list = request_receiver->writeListInit();
  // read_request_list = request_receiver->readListInit();

  // 调度器
  Scheduler scheduler;

  while (currentTime <= maxTime + 105) {
    // 同步时间戳
    currentTime = request_receiver->timestamp_action();
    // 接收请求
    request_receiver->requestAdd(); // 接收器可以通过全局变量操作请求队列
    // 处理请求
    scheduler.myScheduler(); // 一个循环跑一次
  }

  return 0;
}
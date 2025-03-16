#include "globalDefines.h"
#include "globalValue.h"
#include "preProcess.h"
#include "requestRecever.h"
#include "scheduler.h"

int currentTime;
objectList object_list;

int main() {
  // 预处理
  PreProcess preProcess;
  preProcess.run();

  // 请求接收器和三种请求队列
  requestReceiver request_receiver;

  // 调度器
  Scheduler scheduler;

  while (currentTime <= maxTime + 105) {
    // 同步时间戳
    currentTime = request_receiver.timestamp_action();
    // 接收请求
    request_receiver.requestAdd(); // 接收器可以通过全局变量操作请求队列
    // 处理请求
    // scheduler.myScheduler(); // 一个循环跑一次
    // 分开处理请求
    scheduler.myDeleteScheduler();
    scheduler.myWriteScheduler();
    scheduler.myReadScheduler();
  }

  return 0;
}
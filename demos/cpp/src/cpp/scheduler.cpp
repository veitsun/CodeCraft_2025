#include "scheduler.h"
#include "globalValue.h"
#include "handlerDelete.h"
#include "handlerRead.h"
#include "handlerWrite.h"
// #include "object.h"
// #include "objectList.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <iterator>
#include <string>
#include <tuple>
#include <vector>

handlerdelete handleDelete;
handlerwrite handlerWrite;
handlerread handlerRead;

vector<Disk> diskList;

// 一起接收请求并调度分发
//------------------------------------------------------------------------------------------------------
// 分开接收请求并调度分发
void Scheduler::myDeleteScheduler() {
  vector<delRequest> delRequestList = del_request_list.getdelRequest();
  delRequest delRequest;
  vector<int> deleteReadRequest;
  if (delRequestList.size()) {
    for (int currentPos = 0; currentPos < delRequestList.size(); currentPos++) {
      handleDelete.handlerRequestfromScheduler(delRequestList[currentPos]);
    }
    // 先去根据删除请求得到objId，然后再去readNotDone里面根据readRequestId找objId，然后对比objId如果相等则记录次数和readRequestId，循环完成之后进行输出，如果找到了，则也要将readNotDone里面的对应元素删除
    for (int i{0}; i < delRequestList.size(); i++) {
      // 根据删除请求得到了objId
      delRequestList[i].getObjectId();
      // 来遍历readNotDone
      for (auto it = readNotDone.begin(); it != readNotDone.end();) {
        // 根据readNotDone里面的readRequestId获得对应的objId，然后与之前获得的objId对比
        /*
        std::get<0>(*it)是三元组里面的第一个，也就是readRequest
        然后用read_request_list.getreadRequestByRequestId（）根据readRequest获得request对象
        然后再对这个对象使用getObjectId()获得objId再与用delRequest获得的objId做比较
        如果相等则记录并且在readNotDone里面删除
         */
        if (read_request_list.getreadRequestByRequestId(std::get<0>(*it))
                .getObjectId() == delRequestList[i].getObjectId()) {
          deleteReadRequest.emplace_back(std::get<0>(*it));
          it = readNotDone.erase(it);
        } else {
          ++it;
        }
      }
      // readNotDoneUnexpected
      for (auto it = readNotDoneUnexpected.begin();
           it != readNotDoneUnexpected.end();) {
        // 根据readNotDone里面的readRequestId获得对应的objId，然后与之前获得的objId对比
        /*
        std::get<0>(*it)是三元组里面的第一个，也就是readRequest
        然后用read_request_list.getreadRequestByRequestId（）根据readRequest获得request对象
        然后再对这个对象使用getObjectId()获得objId再与用delRequest获得的objId做比较
        如果相等则记录并且在readNotDone里面删除
         */
        if (read_request_list.getreadRequestByRequestId(std::get<0>(*it))
                .getObjectId() == delRequestList[i].getObjectId()) {
          deleteReadRequest.emplace_back(std::get<0>(*it));
          it = readNotDoneUnexpected.erase(it);
        } else {
          ++it;
        }
      }
      for (auto it = readNotRead.begin(); it != readNotRead.end();) {
        if ((*it).getObjectId() == delRequestList[i].getObjectId()) {
          deleteReadRequest.emplace_back((*it).getRequestId());
          it = readNotRead.erase(it);
        } else {
          ++it;
        }
      }
    }
    printf("%d\n", deleteReadRequest.size());
    for (int k{0}; k < deleteReadRequest.size(); ++k) {
      printf("%d\n", deleteReadRequest[k]);
    }
    fflush(stdout);
  } else {
    printf("0\n");
    fflush(stdout);
  }
}

void Scheduler::myWriteScheduler() {
  vector<writeRequest> writeRequestList = write_request_list.getrWriteRequest();
  writeRequest writeRequest;
  for (int currentPos = 0; currentPos < writeRequestList.size(); currentPos++) {
    handlerWrite.handlerRequestfromScheduler(writeRequestList[currentPos]);
    handlerWrite.printCompleteRequest();
  }
  fflush(stdout);
}

void Scheduler::myReadScheduler() {
  int flagggg = -1;
  handlerread handlerRead;
  vector<readRequest> readRequestList = read_request_list.getReadRequest();
  readRequest doLastRead;
  int readFailUnit;
  string printCache;
  // string temp;
  int num;

  // 第一个参数是是否读成功，第二参数是读了多少个对象块，第三个参数是这次读的磁盘号，第四个参数是起始Unit位置，第五个参数是如果这个请求要做未做就true
  tuple<bool, int, int, int, bool> isdone5;
  tuple<bool, int> isdone2;
  int readFailId, readFailObjSize;
  // 对之前时间片中未读完的读请求做读操作
  if (readNotDone.size()) {
    // readNotDone反向循环
    for (auto rit = readNotDone.rbegin(); rit != readNotDone.rend();) {
      // auto &element = readNotDone.back();

      int requestId = std::get<0>(*rit);
      int &requestObjUnit = std::get<1>(*rit);
      int &requestObjSize = std::get<2>(*rit);
      int lastJumpDiskId = std::get<3>(*rit);
      // 调用这个函数就说明了一定能读到，只针对上次跳或者没读完的情况，不针对一次也没读到的情况
      isdone2 = handlerRead.handlerRequestfromScheduler(
          read_request_list.getreadRequestByRequestId(requestId),
          requestObjUnit, requestObjSize, lastJumpDiskId);
      if (std::get<1>(isdone2) == -1) {
        readNotDoneUnexpectedCount++;
        readNotDoneUnexpected.emplace_back((*rit));
        auto it = rit.base();
        // 反向的正向在反向的下一个所以要--来指向要删除的元素
        --it;
        // readNotDone.pop_back();
        it = readNotDone.erase(it);
        rit = std::reverse_iterator<decltype(it)>(it);
      } else if (std::get<0>(isdone2)) {
        // 获得反向迭代器的正向迭代器
        auto it = rit.base();
        // 反向的正向在反向的下一个所以要--来指向要删除的元素
        --it;
        // readNotDone.pop_back();
        it = readNotDone.erase(it);
        rit = std::reverse_iterator<decltype(it)>(it);
      } else {
        requestObjUnit += std::get<1>(isdone2);
        requestObjSize -= std::get<1>(isdone2);
        ++rit;
      }
    }

    // readNotDone正向循环
    //   for (auto it = readNotDone.begin(); it != readNotDone.end();) {
    //     // auto &element = readNotDone.back();

    //     int requestId = std::get<0>(*it);
    //     vector<int> &requestObjUnit = std::get<1>(*it);
    //     int &requestObjSize = std::get<2>(*it);
    //     int lastJumpDiskId = std::get<3>(*it);
    //     isdone2 = handlerRead.handlerRequestfromScheduler(
    //         read_request_list.getreadRequestByRequestId(requestId),
    //         requestObjUnit, requestObjSize, lastJumpDiskId);
    //     if (std::get<0>(isdone2)) {
    //       // readNotDone.pop_back();
    //       it = readNotDone.erase(it);
    //     } else {
    //       for (int n{0}; n < 3; n++) {
    //         // requestObjUnit[n] += isdone.second;
    //         // requestObjSize -= isdone.second;
    //         requestObjUnit[n] += std::get<1>(isdone2);
    //         requestObjSize -= std::get<1>(isdone2);
    //       }
    //       ++it;
    //     }
    //   }
  }

  // 处理上次要做未做的, 想要做 但是 token 不够我做
  if (readNotRead.size()) {
    for (auto rit = readNotRead.rbegin(); rit != readNotRead.rend();) {
      // 调用这个函数就说明了一定能读到，只针对上次跳或者没读完的情况，不针对一次也没读到的情况
      isdone5 = handlerRead.handlerRequestfromScheduler((*rit));
      if (std::get<0>(isdone5)) {
        // 获得反向迭代器的正向迭代器
        auto it = rit.base();
        // 反向的正向在反向的下一个所以要--来指向要删除的元素
        --it;
        // readNotDone.pop_back();
        it = readNotRead.erase(it);
        rit = std::reverse_iterator<decltype(it)>(it);
      }
      // 如果没有读成功
      else if ((!std::get<0>(isdone5))) {
        if (std::get<4>(isdone5)) {
          ++rit;
          // is_arrrive = true;
          // printf("j %d?", .size());
        } else {
          // 获得读失败的当前请求Id
          readFailId = (*rit).getRequestId();
          // 将没有读完的读请求id放入readNotDone这个list中,因为我已经在handler的时候就修改了obj的unit和size,obj不能改
          // 获得读失败，下一个读的起始unit
          Object obj = object_list.getObject((*rit).getObjectId());
          // 这里的qwer是起始Unit位置
          int objSize = obj.getObjectSize();

          readFailUnit = std::get<3>(isdone5) + std::get<1>(isdone5);
          // readFailUnit[m] = objUnit[m] + std::get<1>(isdone3);
          readFailObjSize = objSize - std::get<1>(isdone5);
          readNotDone.emplace_back(make_tuple(
              readFailId, readFailUnit, readFailObjSize, std::get<2>(isdone5)));
          // 获得反向迭代器的正向迭代器
          auto it = rit.base();
          // 反向的正向在反向的下一个所以要--来指向要删除的元素
          --it;
          // readNotDone.pop_back();
          it = readNotRead.erase(it);
          rit = std::reverse_iterator<decltype(it)>(it);
        }
      }
    }
  }

  // 读当前时间片
  if (readRequestList.size()) {
    // 对当前时间片来的读请求做读操作
    for (int currentPos = 0; currentPos < readRequestList.size();
         currentPos++) {
      isdone5 =
          handlerRead.handlerRequestfromScheduler(readRequestList[currentPos]);
      if (!std::get<0>(isdone5)) {
        if (std::get<4>(isdone5)) {
          readNotRead.emplace_back(readRequestList[currentPos]);
        } else {
          // 获得读失败的当前请求Id
          readFailId = readRequestList[currentPos].getRequestId();
          // 将没有读完的读请求id放入readNotDone这个list中,因为我已经在handler的时候就修改了obj的unit和size,obj不能改
          // 获得读失败，下一个读的起始unit
          Object obj =
              object_list.getObject(readRequestList[currentPos].getObjectId());
          // 这里的qwer是起始Unit位置
          int objSize = obj.getObjectSize();

          readFailUnit = std::get<3>(isdone5) + std::get<1>(isdone5);
          // readFailUnit[m] = objUnit[m] + std::get<1>(isdone3);
          readFailObjSize = objSize - std::get<1>(isdone5);
          readNotDone.emplace_back(make_tuple(
              readFailId, readFailUnit, readFailObjSize, std::get<2>(isdone5)));
        }
      }
    }

    //  当j的时候不输出#
    for (int i{0}; i < maxDisk; i++) {
      printCache = diskList[i].getOncetimeDiskHeadAction();
      for (int q{0}; q < handlerRead.whichDiskIJumped.size(); q++) {
        if (handlerRead.whichDiskIJumped[q] == i) {
          flagggg = i;
          break;
        }
      }
      if (i != flagggg) {
        cout << printCache;
        cout << "#";
      } else {
        cout << printCache[0];
        cout << printCache[1];
        string temp;
        for (int u{2}; u < printCache.size(); u++) {
          temp += printCache[u];
        }
        try {
          num = std::stoi(temp);

        } catch (exception &e) {
          cerr << temp << " " << printCache << endl;
          abort();
        }
        cout << num;
      }
      diskList[i].diskPrintCacheClear();
      diskList[i].diskDiskHeadInit();
      cout << endl;
    }
    handlerRead.printCompleteRequest();
  } else {
    for (int i{0}; i < maxDisk; i++) {
      printCache = diskList[i].getOncetimeDiskHeadAction();
      for (int q{0}; q < handlerRead.whichDiskIJumped.size(); q++) {
        if (handlerRead.whichDiskIJumped[q] == i) {
          flagggg = i;
          break;
        }
      }
      if (i != flagggg) {
        cout << printCache;
        cout << "#";
      } else {
        cout << printCache[0];
        cout << printCache[1];
        string temp;
        for (int u{2}; u < printCache.size(); u++) {
          temp += printCache[u];
        }
        try {
          num = std::stoi(temp);

        } catch (exception &e) {
          // cerr << temp << endl;
          cerr << temp << " " << printCache << endl;
          abort();
        }
        cout << num;
      }
      diskList[i].diskPrintCacheClear();
      diskList[i].diskDiskHeadInit();
      cout << endl;
    }
    handlerRead.printCompleteRequest();
  }

  for (int i{0}; i < maxDisk; i++) {
    diskList[i].diskPrintCacheClear();
    diskList[i].diskDiskHeadInit();
  }
  fflush(stdout);
  std::cout.flush();
}
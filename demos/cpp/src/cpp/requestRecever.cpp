#include "requestRecever.h"
#include "globalValue.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
// objectList object_list;
delRequestList del_request_list;
writeRequestList write_request_list;
readRequestList read_request_list;
// 调用指南：先调用三个队列的初始化，再调用时间戳函数，再调用Add函数把指令挂入队列，这样既完成一个时间戳内完整的请求接收操作

int requestReceiver::timestamp_action() {
  // fflush(stdout);
  scanf("%*s%d", &time_stamp);
  printf("TIMESTAMP %d\n", time_stamp);

  fflush(stdout);
  return time_stamp;
}

// 添加请求
void requestReceiver::requestAdd() {
  int id = 0;
  int size = 0;
  int tag = 0;
  int request_id = 0;

  // 删除请求
  scanf("%d", &request_num);
  if (request_num == 0) { // 没有删除操作，注意需要输出一个0
    printf("0\n");
    fflush(stdout);
  } else {
    // 逐行读取指令并添加入队列
    for (; request_num > 0; request_num--) {
      scanf("%d", &id);
      unique_ptr<delRequest> request_now = make_unique<delRequest>();
      request_now->setObjectId(id);
      request_now->setTime(time_stamp);
      del_request_list.addDelRequest(*request_now);
    }
  }

  // 写入请求
  scanf("%d", &request_num);
  if (request_num == 0)
    ; // 不做任何操作
  else {
    for (; request_num > 0; request_num--) {
      scanf("%d%d%d", &id, &size, &tag);
      unique_ptr<writeRequest> request_now = make_unique<writeRequest>();
      request_now->setObjectId(id);
      request_now->setObjectSize(size);
      request_now->setObjectTag(tag);
      request_now->setTime(time_stamp);
      write_request_list.addWriteRequest(*request_now);
    }
  }

  // 读取请求
  scanf("%d", &request_num);
  if (request_num == 0)
    ; // 不做任何操作
  else {
    for (; request_num > 0; request_num--) {
      scanf("%d%d", &request_id, &id);
      unique_ptr<readRequest> request_now = make_unique<readRequest>();
      request_now->setRequestId(request_id);
      request_now->setObjectId(id);
      request_now->setTime(time_stamp);
      read_request_list.addReadRequest(*request_now);
    }
  }
  // 读取完毕
}

// 调用测试代码
// int main() {
//   requestReceiver *request_receiver = new requestReceiver();
//   request_receiver->timestamp_action();
//   delRequestList *del_request_list = request_receiver->delListInit();
//   writeRequestList *write_request_list = request_receiver->writeListInit();
//   readRequestList *read_request_list = request_receiver->readListInit();
//   request_receiver->requestAdd(del_request_list, write_request_list,
//                                read_request_list);
//   printf("%d\n", write_request_list->getList()->getObjectId());
// }
// 调用测试代码
//  int main()
//  {
//      requestReceiver* request_receiver = new requestReceiver();
//      request_receiver -> timestamp_action();
//      delRequestList* del_request_list = request_receiver -> delListInit();
//      writeRequestList* write_request_list = request_receiver ->
//      writeListInit(); readRequestList* read_request_list = request_receiver
//      -> readListInit(); printf("begin add\n"); request_receiver ->
//      requestAdd(del_request_list, write_request_list, read_request_list);
//      printf("%d\n", write_request_list -> outList() -> getObjectId());
//  }
#include "requestRecever.h"

#include <cstdio>
#include <cassert>
#include <cstdlib>

//调用指南：先调用三个队列的初始化，再调用时间戳函数，再调用Add函数把指令挂入队列，这样既完成一个时间戳内完整的请求接收操作

delRequestList* requestReceiver::delListInit()
{
    delRequestList* del_request_list = new delRequestList();
    return del_request_list;
}

writeRequestList* requestReceiver::writeListInit()
{
    writeRequestList* write_request_list = new writeRequestList();
    return write_request_list;
}

readRequestList* requestReceiver::readListInit()
{
    readRequestList* read_request_list = new readRequestList();
    return read_request_list;
}
void requestReceiver::timestamp_action()
{
    scanf("%*s%d", &time_stamp);
    printf("TIMESTAMP %d\n", time_stamp);

    fflush(stdout);
}


// 添加请求
void requestReceiver::requestAdd(delRequestList* del_request_list, writeRequestList* write_request_list, readRequestList* read_request_list)
{
    int id = 0;
    int size = 0;
    int tag = 0;
    int request_id = 0;
    //先读删除，再读写入，最后读的是读取
    scanf("%d", &request_num);
    if(request_num == -1)
        return;//没读到东西，后续再考虑这里
    if(request_num == 0)//没有删除操作，注意需要输出一个0
        printf("0\n");
    else
    {
        //逐行读取指令并添加入队列
        for(;request_num > 0; request_num--){
            scanf("%d", &id);
            delRequest* request_now = new delRequest();
            request_now -> setObjectId(id);
            request_now -> setTime(time_stamp);
            del_request_list -> setList(request_now);
        }
    }
    //写入
    scanf("%d", &request_num);
    if(request_num == 0)
        ;//不做任何操作
    else
    {
        for(;request_num > 0; request_num--){
            scanf("%d%d%d", &id, &size, &tag);
            writeRequest* request_now = new writeRequest();
            request_now -> setObjectId(id);
            request_now -> setObjectSize(size);
            request_now -> setObjectTag(tag);
            request_now -> setTime(time_stamp);
            write_request_list -> setList(request_now);
        }
    }
    //读取
    scanf("%d", &request_num);
    if(request_num == 0)
        ;//不做任何操作
    else
    {
        for(;request_num > 0; request_num--){
            scanf("%d%d", &request_id, &id);
            readRequest* request_now = new readRequest();
            request_now -> setRequestId(request_id);
            request_now -> setObjectId(id);
            request_now -> setTime(time_stamp);
            read_request_list -> setList(request_now);
        }
    }
    //读取完毕
   
}

//调用测试代码
// int main()
// {
//     requestReceiver* request_receiver = new requestReceiver();
//     request_receiver -> timestamp_action();
//     delRequestList* del_request_list = request_receiver -> delListInit();
//     writeRequestList* write_request_list = request_receiver -> writeListInit();
//     readRequestList* read_request_list = request_receiver -> readListInit();
//     printf("begin add\n");
//     request_receiver -> requestAdd(del_request_list, write_request_list, read_request_list);
//     printf("%d\n", write_request_list -> outList() -> getObjectId());
// }
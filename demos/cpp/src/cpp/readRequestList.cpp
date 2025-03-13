#include "readRequestList.h"

void readRequestList::addList(readRequest *request){
    read_request_list.push_back(request);//放在队尾
}
readRequest* readRequestList::outList(){
    //弹出队头
    readRequest *request = read_request_list.front();
    read_request_list.pop_front();
    return request;
}

bool readRequestList::listIsEmpty(){
    return read_request_list.empty();
}
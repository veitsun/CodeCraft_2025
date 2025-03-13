#include "readRequestList.h"

void readRequestList::setList(readRequest *request){
    read_request_list.push_back(request);//放在队尾
}

bool readRequestList::listIsEmpty(){
    return read_request_list.empty();
}

readRequest* readRequestList::getList(){
    if (readRequestList::listIsEmpty())
        return nullptr;
    //弹出队头
    readRequest *request = read_request_list.front();
    read_request_list.pop_front();
    return request;
}
#include "writeRequestList.h"

void writeRequestList::setList(writeRequest *request){
    write_request_list.push_back(request);//放在队尾
}

bool writeRequestList::listIsEmpty(){
    return write_request_list.empty();
}

writeRequest* writeRequestList::getList(){
    if(writeRequestList::listIsEmpty())
        return nullptr;
    //弹出队头
    writeRequest *request = write_request_list.front();
    write_request_list.pop_front();
    return request;
}
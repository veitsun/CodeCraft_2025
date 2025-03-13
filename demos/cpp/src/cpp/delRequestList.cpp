#include "delRequestList.h"

void delRequestList::setList(delRequest *request){
    del_request_list.push_back(request);//放在队尾
}

bool delRequestList::listIsEmpty(){
    return del_request_list.empty();
}

delRequest* delRequestList::getList(){
    if(delRequestList::listIsEmpty())
        return nullptr;
    //弹出队头
    delRequest *request = del_request_list.front();
    del_request_list.pop_front();
    return request;
}
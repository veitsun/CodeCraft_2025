#include "delRequestList.h"

void delRequestList::addList(delRequest *request){
    del_request_list.push_back(request);//放在队尾
}
delRequest* delRequestList::outList(){
    //弹出队头
    delRequest *request = del_request_list.front();
    del_request_list.pop_front();
    return request;
}

bool delRequestList::listIsEmpty(){
    return del_request_list.empty();
}
#include "delRequestList.h"

void delRequestList::setList(delRequest *request){
    del_request_list.push_back(request);//放在队尾
}

bool delRequestList::listIsEmpty(){
    return del_request_list.empty();
}

std::vector<delRequest*> delRequestList::getList(){
    vector<delRequest*> request;
    while(!delRequestList::listIsEmpty()){
        //弹出队头
        request.push_back(del_request_list.front());
        del_request_list.pop_front();
    }
    return request;
}
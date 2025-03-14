#include "readRequestList.h"

void readRequestList::setList(readRequest *request){
    read_request_list.push_back(request);//放在队尾
}

bool readRequestList::listIsEmpty(){
    return read_request_list.empty();
}

vector<readRequest*> readRequestList::getList(){
    std::vector<readRequest*> request;
    while(!readRequestList::listIsEmpty()){
        // 弹出队头dd
        request.push_back(read_request_list.front());
        read_request_list.pop_front();
    }
    return request;
}
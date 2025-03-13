#include "writeRequestList.h"

void writeRequestList::addList(writeRequest *request){
    write_request_list.push_back(request);//放在队尾
}
writeRequest* writeRequestList::outList(){
    //弹出队头
    writeRequest *request = write_request_list.front();
    write_request_list.pop_front();
    return request;
}

bool writeRequestList::listIsEmpty(){
    return write_request_list.empty();
}
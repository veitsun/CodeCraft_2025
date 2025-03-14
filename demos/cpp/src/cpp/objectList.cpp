#include "object.h"


//创建obj对象，把对象挂入对象表
void objectList::setList(int object_id, int tag_id, int object_size, int* disk_num, int* unit)
{
    if(object_id > objectList::listSize())
        object_list.resize(object_id + 1000);
    Object* obj = new Object();
    obj->setObject(object_id, tag_id, object_size, disk_num, unit);
    object_list[object_id] = obj;
}

void objectList::deleteObject(int obj_id)
{
    delete(object_list[obj_id]);
    object_list[obj_id] = nullptr;
}

#pragma once

#include <list>
#include <vector>
#include "object.h"

using namespace std;

//本实现支持异步，也就是obj_id不是按照顺序来的情况下，也可以把指针放在id对应的下标下
//还可以考虑用map来实现，到时候可以测试一下

class objectList
{
    //用于对应上object对象实体的索引表
    private:
        vector<Object*> object_list;
    public:
        objectList(){object_list.reserve(1000);}
        int listSize(){return object_list.size();}
        void setList(int object_id, int tag_id, int object_size, int* disk_num, int* unit){}
        Object* getList(int obj_id){return object_list[obj_id];}

        int getObjectId(int obj_id){return getList(obj_id) -> Object::getObjectId();}
        int getObjectTag(int obj_id){return getList(obj_id) -> Object::getObjectTag();}
        int getObjectSize(int obj_id){return getList(obj_id) -> Object::getObjectSize();}
        int* getObjectDisk(int obj_id){return getList(obj_id) -> Object::getObjectDisk();}
        int* getObjectUnit(int obj_id){return getList(obj_id) -> Object::getObjectUnit();}

        void deleteObject(int obj_id){}
        void clean(){}
};
#include "objectList.h"

// 创建obj对象，把对象挂入对象表
void objectList::addObject(Object obj) {
  int id = obj.getObjectId();
  if (id > object_list.size())
    object_list.resize(id + 1000);
  object_list[id] = obj;
}

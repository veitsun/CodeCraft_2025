#pragma once

#include "object.h"
#include <list>
#include <vector>

using namespace std;

// 本实现支持异步，也就是obj_id不是按照顺序来的情况下，也可以把指针放在id对应的下标下
// 还可以考虑用map来实现，到时候可以测试一下

class objectList {
  // 用于对应上object对象实体的索引表
private:
  vector<Object> object_list;

public:
  objectList() { object_list.reserve(1000); }
  void addObject(Object object);
  Object getObject(int obj_id) {return object_list[obj_id];}
};
#include "object.h"

void Object::setObject(int object_id, int tag_id, int object_size,
                       int *disk_num, int *unit) {
  this->object_id = object_id;
  this->tag_id = tag_id;
  this->object_size = object_size;
  for (int i = 0; i < REP_NUM + 1; i++) {
    this->disk_num[i] = disk_num[i];
    this->unit[i] = unit[i];
  }
}

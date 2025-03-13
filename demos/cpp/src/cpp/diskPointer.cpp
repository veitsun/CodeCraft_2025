#include "diskPointer.h"

DiskPointer::DiskPointer(int token) {
  current_position = 1;
  this->token = token; // G 是令牌信息，全局变量
}

DiskPointer::~DiskPointer() {}
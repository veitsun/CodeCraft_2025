// #include "globalDefines.h"
#include "diskPointer.h"
#include "globalValue.h"

DiskPointer::DiskPointer() {
  current_position = 1;
  this->token = maxToken; // G 是令牌信息，全局变量
  pre_is_read = false;
  pre_token = 0;
  read_nums = 0;
}

DiskPointer::~DiskPointer() {}
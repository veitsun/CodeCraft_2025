#pragma once

class DiskUnit {
private:
  // int object_id;
  // int object_block_id;
  // bool is_deleted;

  // int tag_id;

public:
  int object_id;
  int object_block_id;
  bool is_deleted;
  int tag_id;
  bool is_used;
  // virtual void clear() = 0;
  DiskUnit() {
    object_id = 0;
    object_block_id = 0;
    is_deleted = false;
    is_used = false;
    tag_id = 0;
  }
};

// class DiskUnitImplement1 : public DiskUnit {
// private:
// public:
//   void clear() {};
// };
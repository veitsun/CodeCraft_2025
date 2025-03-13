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
  // virtual void clear() = 0;
};

// class DiskUnitImplement1 : public DiskUnit {
// private:
// public:
//   void clear() {};
// };
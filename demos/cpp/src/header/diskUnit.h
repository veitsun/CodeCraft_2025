#pragma once

class DiskUnit {
private:
  int objectId;
  int used;

public:
  virtual void clear() = 0;
};

class DiskUnitImplement1 : public DiskUnit {
private:
public:
  void clear() {};
};
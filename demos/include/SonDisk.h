#pragma once
#include <vector>

using namespace std;
class SonDisk {
private:
public:
  int tag_id;
  // vector<pair<int, int>> free_space;
  vector<bool> storage;
  SonDisk() { this->tag_id = 0; }
  void initSonDisk(int tag_id, int length) {
    this->tag_id = tag_id;

    storage.resize(length);
    for (int i = 0; i < storage.size(); i++) {
      storage[i] = false;
    }
  }
};

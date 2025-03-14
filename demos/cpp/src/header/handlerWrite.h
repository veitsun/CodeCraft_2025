#pragma once
#include <utility>
#include <vector>
using namespace std;
class writeHeadler {
private:
  /* data */
public:
  writeHeadler(/* args */) {};
  ~writeHeadler() {};
  vector<vector<std::pair<int, int>>> writeToDisk();
};
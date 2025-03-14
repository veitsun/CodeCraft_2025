#include "handlerWrite.h"
#include "disk.h"
#include <utility>
#include <vector>

// std::vector<vector<std::pair<int, int>>> writeHeadler::writeHeadler() {
//   vector<Disk> diskList;

//   return diskList[0].
// }

Disk disk;

// 直接发给Disk，告诉Disk从哪开始放，要放多少个
void handlerwrite::handlerWrite2Disk(int unitStart, int objId, int objSize) {
  disk.diskWrite(unitStart, objId, objSize);
}

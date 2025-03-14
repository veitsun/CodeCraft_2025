#include "delRequestList.h"
#include "disk.h"
#include "object.h"
#include "readRequestList.h"
#include "writeRequestList.h"

// 公开的信息
extern vector<Disk> diskList;

extern vector<Object> objectList;
extern readRequestList read_request_list;
extern writeRequestList write_request_list;
extern delRequestList del_request_list;
extern int maxTime, maxTag, maxDisk, maxDiskSize, maxToken;

extern vector<vector<vector<int>>> actionOnBlockCount;
extern vector<int> maxSpaceForTag;
// ! tagDistributeInAllDisk[1] 才是第一个磁盘
extern vector<TagDistributeInDisk> tagDistributeInAllDisk;
extern vector<tuple<int, int, int>> tagRepID;
#pragma once
#include "delRequestList.h"
#include "disk.h"
#include "objectList.h"
#include "readRequestList.h"
#include "writeRequestList.h"

// 全局声明
extern vector<Disk> diskList;

extern objectList object_list;
extern readRequestList read_request_list;
extern writeRequestList write_request_list;
extern delRequestList del_request_list;
extern int maxTime, maxTag, maxDisk, maxDiskSize, maxToken;

/*
  <0, 0, 100> 代表 tag0 存储在当前磁盘的 0~100
*/
typedef tuple<int, int, int> TagDistribute;
/*
  对于一个磁盘，它的 tag 分配是这样的
  <0, 0, 100>
  <1, 100, 200>
  <2, 200, 300>
*/
typedef vector<tuple<int, int, int>> TagDistributeInDisk;
/*
  action_on_block_count[i][j][k]
    i： 代表操作类型 delete、write、read
    j： 代表标签类型
    k： 代表大时间片
*/
extern vector<vector<vector<int>>> actionOnBlockCount;
extern vector<int> maxSpaceForTag;
// ! tagDistributeInAllDisk[1] 才是第一个磁盘
extern vector<TagDistributeInDisk> tagDistributeInAllDisk;
extern vector<vector<int>> tagRepID;
extern int currentTime;
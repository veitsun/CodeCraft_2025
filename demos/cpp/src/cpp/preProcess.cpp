#include "preProcess.h"

#include "globalDefines.h"

#include <iostream>

using namespace std;

int maxTime, maxTag, maxDisk, maxDiskSize, maxToken;
/*
  action_on_block_count[i][j][k]
    i： 代表操作类型 delete、write、read
    j： 代表标签类型
    k： 代表大时间片
*/
vector<vector<vector<int>>> actionOnBlockCount;

vector<int> maxSpaceForTag;
// ! tagDistributeInAllDisk[1] 才是第一个磁盘
vector<TagDistributeInDisk> tagDistributeInAllDisk;
// tagRepID[0] = <1, 2, 3> 代表 tag0 在 1, 2, 3 磁盘上的备份
// tagRepID[1] = <5, 6, 7> 代表 tag1 在 5, 6, 7 磁盘上的备份
vector<vector<int>> tagRepID;

// --------------------public--------------------

int PreProcess::run() {
  scanf("%d%d%d%d%d", &maxTime, &maxTag, &maxDisk, &maxDiskSize, &maxToken);
  // 初始化全局变量
  int maxBigTime = (maxTime - 1) / FRE_PER_SLICING;
  tagDistributeInAllDisk.resize(maxDisk + 1);
  actionOnBlockCount.resize(ACITON_TYPE_COUNT);
  maxSpaceForTag.resize(maxTag);
  tagRepID.resize(maxTag);

  // 读取预处理数据
  for (int action = 0; action < ACITON_TYPE_COUNT; action++) {
    actionOnBlockCount[action].resize(maxTag);
    for (int tag = 0; tag < maxTag; tag++) {
      actionOnBlockCount[action][tag].resize(maxBigTime + 1);
      for (int bigTime = 0; bigTime <= maxBigTime; bigTime++) {
        scanf("%d", &actionOnBlockCount[action][tag][bigTime]);
      }
    }
  }

  // 计算每种tag最大需要的空间
  vector<int> rawMaxSpaceForTag;
  rawMaxSpaceForTag.resize(maxTag);

  int maxSpaceForAllTag = 0;
  for (int tag = 0; tag < maxTag; tag++) {
    rawMaxSpaceForTag[tag] = actionOnBlockCount[WRITE][tag][0];
    for (int bigTime = 1; bigTime <= maxBigTime; bigTime++) {
      rawMaxSpaceForTag[tag] += actionOnBlockCount[WRITE][tag][bigTime] -
                                actionOnBlockCount[DELETE][tag][bigTime - 1];
    }
    maxSpaceForAllTag += rawMaxSpaceForTag[tag];
  }

  // 伸缩空间需求
  double fixRadio = maxDisk * maxDiskSize / maxSpaceForAllTag * REP_NUM;
  if (fixRadio < 1) {
    for (int tag = 0; tag < maxTag; tag++) {
      maxSpaceForTag[tag] = fixRadio * rawMaxSpaceForTag[tag];
    }
  }

  // diskUsedSpace 记录磁盘已分配空间
  vector<int> diskUsedSpace;
  diskUsedSpace.resize(maxDisk + 1);
  for (int diskId = 1; diskId <= maxDisk; diskId++) {
    diskUsedSpace[diskId] = 0;
  }

  // haveDistributeTag记录tag是否已分配到磁盘
  vector<bool> haveDistributeTag;
  haveDistributeTag.resize(maxTag);

  // 依次为tag分配一组磁盘
  for (int tag = 0; tag < maxTag; tag++) {
    // 寻找合适的一组连续的磁盘
    for (int currentDiskGroup = 1; currentDiskGroup * REP_NUM <= maxDisk;
         currentDiskGroup++) {
      int tempDiskUsedSpace =
          diskUsedSpace[currentDiskGroup] + maxSpaceForTag[tag];

      if (tempDiskUsedSpace <= maxDiskSize) {
        // tagDistribute 就是一个tag的范围
        TagDistribute tagDistribute = make_tuple(
            tag, tempDiskUsedSpace - maxSpaceForTag[tag], tempDiskUsedSpace);

        haveDistributeTag[tag] = true;

        int maxDupDiskID = currentDiskGroup * REP_NUM; // 3个备份磁盘 最大磁盘号
        for (int dupNum = 0; dupNum < REP_NUM; dupNum++) {
          tagRepID[tag].push_back(maxDupDiskID);
          diskUsedSpace[maxDupDiskID] = tempDiskUsedSpace;
          tagDistributeInAllDisk[maxDupDiskID].push_back(tagDistribute);
          maxDupDiskID--;
        }
      } else {
        // 磁盘空间不足
        currentDiskGroup++;
      }
    }
  }

  // 对于没有分配的tag，寻找不连续的磁盘存放
  // ! 不保证能分好
  // int lastDiskID = maxDisk % REP_NUM;

  // 一个tag存放的磁盘编号
  vector<int> allRepDiskID;
  for (int tag = 0; tag < maxTag; tag++) {
    // 还没有分配的tag
    if (!haveDistributeTag[tag]) {
      allRepDiskID.clear();
      int rep_num = 0;
      for (int diskID = maxDisk; diskID >= 1; diskID--) {
        int tempDiskUsedSpace = diskUsedSpace[diskID] + maxSpaceForTag[tag];
        if (tempDiskUsedSpace <= maxDiskSize) {
          allRepDiskID.push_back(diskID);
          rep_num++;
        }
        // 找到3个备份磁盘
        if (rep_num == REP_NUM) {
          for (int repCount = 0; repCount < REP_NUM; repCount++) {
            TagDistribute tagDistribute =
                make_tuple(tag, tempDiskUsedSpace - maxSpaceForTag[tag],
                           tempDiskUsedSpace);
            diskUsedSpace[allRepDiskID[repCount]] = tempDiskUsedSpace;
            tagDistributeInAllDisk[diskID].push_back(tagDistribute);
          }
          haveDistributeTag[tag] = true;
        }
      }
    }
  }

  for (int tag = 0; tag < maxTag; tag++) {
    if (!haveDistributeTag[tag]) {
      cout << "tag " << tag << "没有分配" << endl;
    }
  }

  return 0;
}

// --------------------public-------------------
#include "preProcess.h"

#include "globalDefines.h"
#include "globalValue.h"

#include <cstdio>
#include <iostream>
#include <tuple>

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

/*
  Tag：从 0 开始
  Disk：从 1 开始
*/

// 读取预处理数据
void PreProcess::acceptInput() {
  scanf("%d %d %d %d %d", &maxTime, &maxTag, &maxDisk, &maxDiskSize, &maxToken);
  int maxBigTime = (maxTime - 1) / FRE_PER_SLICING;

  tagDistributeInAllDisk.resize(maxDisk + 1);
  actionOnBlockCount.resize(ACITON_TYPE_COUNT);
  maxSpaceForTag.resize(maxTag);
  tagRepID.resize(maxTag);
  diskList.resize(maxDisk);

  for (int action = 0; action < ACITON_TYPE_COUNT; action++) {
    actionOnBlockCount[action].resize(maxTag);
    for (int tag = 0; tag < maxTag; tag++) {
      actionOnBlockCount[action][tag].resize(maxBigTime + 1);
      for (int bigTime = 0; bigTime <= maxBigTime; bigTime++) {
        scanf("%d", &actionOnBlockCount[action][tag][bigTime]);
      }
    }
  }
  return;
}

void PreProcess::calculateTagSpace() {
  int maxBigTime = (maxTime - 1) / FRE_PER_SLICING;
  // 存储每种tag最大需要的空间
  vector<int> rawMaxSpaceForTag;
  rawMaxSpaceForTag.resize(maxTag);
  int maxSpaceForAllTag = 0;

  for (int tag = 0; tag < maxTag; tag++) {
    maxSpaceForTag[tag] = 0;
    int tempSpaceNeed =
        actionOnBlockCount[WRITE][tag][0] - actionOnBlockCount[DELETE][tag][0];
    for (int bigTime = 1; bigTime <= maxBigTime; bigTime++) {
      tempSpaceNeed += actionOnBlockCount[WRITE][tag][bigTime] -
                       actionOnBlockCount[DELETE][tag][bigTime];
      if (rawMaxSpaceForTag[tag] < tempSpaceNeed) {
        rawMaxSpaceForTag[tag] = tempSpaceNeed;
      }
    }
    maxSpaceForAllTag += rawMaxSpaceForTag[tag];
  }

  // 伸缩空间需求
  double fixRadio =
      (maxDisk * maxDiskSize * 1.0) / (maxSpaceForAllTag * REP_NUM);
  if (fixRadio < 1) {
    for (int tag = 0; tag < maxTag; tag++) {
      maxSpaceForTag[tag] = fixRadio * rawMaxSpaceForTag[tag];
    }
  }
  return;
}

void PreProcess::allockDiskGroupSpaceForTag() {
  vector<int> TagsSpaceNeedInDisk;
  TagsSpaceNeedInDisk.resize(maxTag);
  int spaceUsed = 0;
  for (int tag = 0; tag < maxTag; tag++) {
    TagsSpaceNeedInDisk[tag] = maxSpaceForTag[tag] / maxDisk * REP_NUM;
    spaceUsed += TagsSpaceNeedInDisk[tag];
    TagDistribute tagDistribute =
        make_tuple(tag, spaceUsed - TagsSpaceNeedInDisk[tag], spaceUsed);
    for (int diskID = 1; diskID <= maxDisk; diskID++) {
      tagDistributeInAllDisk[diskID].push_back(tagDistribute);
    }
  }

  return;
}

// --------------------public--------------------

int PreProcess::run() {
  // 读取输入
  acceptInput();

  // 计算tag所需空间
  calculateTagSpace();

  // 依次为tag分配一组磁盘
  allockDiskGroupSpaceForTag();

  printf("OK\n");
  fflush(stdout);

  return 0;
}

// --------------------public-------------------
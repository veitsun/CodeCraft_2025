#include "preProcess.h"

#include <iostream>

using namespace std;

// --------------------private--------------------

void PreProcess::AcceptGlobleParameter(void) {
  scanf("%d%d%d%d%d", &this->maxTime, &this->maxTag, &this->maxDisk,
        &this->maxDiskSize, &this->maxToken);
}

// --------------------private--------------------

// --------------------public--------------------

PreProcess::PreProcess() { AcceptGlobleParameter(); };

// --------------------public-------------------
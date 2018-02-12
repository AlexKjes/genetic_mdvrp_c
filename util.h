//
// Created by alex on 2/6/18.
//

#include "mdvrp.h"

#ifndef ASSIGN1_C_UTIL_H
#define ASSIGN1_C_UTIL_H


void initRand();

int getRandInt();

void randIntArr(int low, int high, int size, int* buffer);

int randComp(const void* i1, const void* i2);

double normalRand();

int closestDepot(MDVRP* mdvrp, int customer);

#endif //ASSIGN1_C_UTIL_H





//
// Created by alex on 2/5/18.
//

#ifndef ASSIGN1_C_GENETICS_H
#define ASSIGN1_C_GENETICS_H

#include "mdvrp.h"

typedef struct {

    int m;
    int n;

    int* matrix;

} Genotype;

// utils
Genotype* initGenotype(MDVRP* mdvrp);
int routeMax(MDVRP* mdvrp);
int customerDemandComp(const void* customer1, const void* customer2);

Genotype* makeRandomSpecimen(MDVRP mdvrp);


#endif //ASSIGN1_C_GENETICS_H

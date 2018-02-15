//
// Created by alex on 2/7/18.
//

#ifndef ASSIGN1_C_OUTPUT_H
#define ASSIGN1_C_OUTPUT_H

#include "mdvrp.h"
#include "genetics.h"

void initGNUPlot(int genomeLen, int fileEnum);
void drawMDVRP(MDVRP* mdvrp);
void drawSpecimen(MDVRP* mdvrp, Genotype* genotype);
void printSpecs(MDVRP* mdvrp, Genotype* specimen);

#endif //ASSIGN1_C_OUTPUT_H

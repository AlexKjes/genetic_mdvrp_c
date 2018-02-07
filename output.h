//
// Created by alex on 2/7/18.
//

#ifndef ASSIGN1_C_OUTPUT_H
#define ASSIGN1_C_OUTPUT_H

#include "mdvrp.h"
#include "genetics.h"

void initGNUPlotPipe();
void drawMDVRP(MDVRP* mdvrp);
void drawSpecimen(MDVRP* mdvrp, Genotype* genotype);

#endif //ASSIGN1_C_OUTPUT_H

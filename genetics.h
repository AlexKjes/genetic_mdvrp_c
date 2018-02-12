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
void printSpecimen(Genotype* specimen);
int routeMax(MDVRP* mdvrp);

/// comparators
int customerDemandComp(const void* customer1, const void* customer2);
int parentOrderComp(const void* p1, const void* p2);

// population creation
Genotype* initGenotype(MDVRP* mdvrp);
Genotype* makeRandomSpecimen(MDVRP* mdvrp);
Genotype** generateRandomPopulation(MDVRP* mdvrp, int size);
void nextGeneration(MDVRP* mdvrp, int elitism, double mutationRate, int crossover, int popSize, double* fitness, Genotype** population);
Genotype* cloneGenotype(Genotype* specimen);
// population destruction
void destroyPopulation(int popSize, Genotype** population);
void destroySpecimen(Genotype* Specimen);


// population manipulation
/**
 * @param popSize population size
 * @param fitness population fitness array
 * @param parents array of parents indices. Twice the size of population. [parent1, paren2, parent1, paren2...]
 */
void crossoverSelection(int popSize, double* fitness, int* parents);

Genotype* crossover(MDVRP* mdvrp, Genotype* p1, Genotype* p2);


// mutation
void mutatePopulation(MDVRP* mdvrp, double mutationRate, int popSize, Genotype** population);
void swapMutate(MDVRP *mdvrp, Genotype *specimen);
void shuffleSegmentMutate(MDVRP *mdvrp, Genotype *specimen);
void swapTruckMutate(MDVRP *mdvrp, Genotype *specimen);
void swapInRouteMutate(MDVRP *mdvrp, Genotype *specimen);


// validation
int validateTruck(MDVRP* mdvrp, int truck, Genotype* genotype);
int validateSpecimen(MDVRP* mdvrp, Genotype* genotype);

// calculations
double calculateTruckDistance(MDVRP* mdvrp, int truck, Genotype* genotype);
double calculateSpecimenDistance(MDVRP* mdvrp, Genotype* genotype);
int calculateTruckLoad(MDVRP* mdvrp, int truck, Genotype* genotype);
int calculateSpecimenLoad(MDVRP* mdvrp, Genotype* genotype);
void calculateFitness(MDVRP* mdvrp, Genotype** population, int popSize, double* fitness);

#endif //ASSIGN1_C_GENETICS_H

//
// Created by alex on 2/5/18.
//

#ifndef ASSIGN1_C_DATA_READER_H
#define ASSIGN1_C_DATA_READER_H


#include <stdlib.h>
#include <stdio.h>

typedef struct {

    int x;
    int y;
    int demand;

} Customer;

typedef struct {

    int x;
    int y;
    int maxLoad;
    int maxRoute;
} Depot;


typedef struct {

    int nCustomers;
    int nDepots;
    int trucksPerDepot;

    Customer* customers;
    Depot* depots;

} MDVRP ;


MDVRP* loadProblem(char* filePath);
MDVRP* initMDVRP(int nCustomers, int nDepots);


#endif //ASSIGN1_C_DATA_READER_H

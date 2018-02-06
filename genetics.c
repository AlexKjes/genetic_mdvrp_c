//
// Created by alex on 2/5/18.
//

#include "genetics.h"
#include "util.h"


/// return nCustomers on longest possible route
int routeMax(MDVRP* mdvrp){
    // copy and sort customers by demand
    Customer sortCustomers[mdvrp->nCustomers];
    for (int i=0;i<mdvrp->nCustomers;i++){
        sortCustomers[i] = mdvrp->customers[i];
    }
    qsort(sortCustomers, (size_t)mdvrp->nCustomers, sizeof(Customer), customerDemandComp);
    // find largest truck
    int maxLoad = 0;
    for (int i=0;i<mdvrp->nDepots;i++){
        if (mdvrp->depots[i].maxLoad > maxLoad){
            maxLoad = mdvrp->depots[i].maxLoad;
        }
    }

    // calculate max route length
    int acc = 0;
    int i;
    for (i=0;i<mdvrp->nCustomers;i++){
        acc += sortCustomers[i].demand;
        if (acc > maxLoad)
            break;
    }
    return i;
}

/// comparator for customers
int customerDemandComp(const void* customer1, const void* customer2){
    Customer c1 = *((Customer*)customer1);
    Customer c2 = *((Customer*)customer2);
    if (c1.demand > c2.demand)
        return 1;
    if (c1.demand < c2.demand)
        return -1;
    return 0;
}



/// make a genotype
Genotype* initGenotype(MDVRP* mdvrp){

    Genotype* g = malloc(sizeof(Genotype));
    g->m = mdvrp->nDepots * mdvrp->trucksPerDepot;
    g->n = routeMax(mdvrp);

    int mSize = g->m * g->n;
    g->matrix = malloc(sizeof(int) * mSize);

    for (int i=0;i<mSize;i++){
        g->matrix[i] = -1;
    }

    return g;
}


Genotype* makeRandomSpecimen(MDVRP* mdvrp){



    Genotype* g = initGenotype(mdvrp);

    int score[g->m];
    for (int i=0;i<g->m;i++) { score[i]=0; }

    int shuffle[mdvrp->nCustomers];
    for (int i=0;i<mdvrp->nCustomers;i++) { shuffle[i]=i; }
    qsort(shuffle, (size_t)mdvrp->nCustomers, sizeof(int), randComp);
    printf("yolo\n");
    for (int i=0;i<mdvrp->nCustomers;i++){
        printf("%d\n", shuffle[i]);
    }
    for (int i=0;i<mdvrp->nCustomers;i++){
        int rnd = rand() % g->m;
    }

    return g;

}
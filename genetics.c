//
// Created by alex on 2/5/18.
//

#include <math.h>
#include <string.h>
#include "genetics.h"
#include "util.h"
#include "mdvrp.h"


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


Genotype*  makeRandomSpecimen(MDVRP* mdvrp){

    Genotype* g = initGenotype(mdvrp);
    // Number of customers per truck
    int routeLengths[g->m];
    for (int i=0;i<g->m;i++) { routeLengths[i]=0; }
    // Shuffled customer indices
    int shuffle[mdvrp->nCustomers];
    for (int i=0;i<mdvrp->nCustomers;i++) { shuffle[i]=i; }
    qsort(shuffle, (size_t)mdvrp->nCustomers, sizeof(int), randComp);
    // shuffled truck indices
    int rnd[g->m];
    for (int i=0;i<g->m;i++){ rnd[i] = i; }
    for (int i=0;i<mdvrp->nCustomers;i++){
        qsort(rnd, (size_t)g->m, sizeof(int), randComp);
        for (int j=0;j<g->m;j++){
            g->matrix[rnd[j]*g->n+routeLengths[rnd[j]]] = shuffle[i];
            if (routeLengths[rnd[j]] < g->n){
                routeLengths[rnd[j]]++;
                break;
            }
            g->matrix[rnd[j]*g->n+routeLengths[rnd[j]]] = -1;

        }
    }

    return g;

}


Genotype** generateRandomPopulation(MDVRP* mdvrp, int size){
    Genotype** pop = malloc(sizeof(Genotype*)*size);
    for (int i=0;i<size;i++){
        pop[i] = makeRandomSpecimen(mdvrp);
    }
    return pop;
}



/// Test if a truck satisfies hard constraints
int validateTruck(MDVRP* mdvrp, int truck, Genotype* genotype){

    double dist = 0;
    double load = 0;
    int depot = truck/mdvrp->trucksPerDepot;
    int pos[2] = {mdvrp->depots[depot].x, mdvrp->depots[depot].y};
    for (int i=0;i<genotype->n;i++){
        if (genotype->matrix[truck*genotype->n+i] != -1) {
            int c = genotype->matrix[truck*genotype->n+i];
            dist += sqrt((pow(pos[0] - mdvrp->customers[c].x, 2) + pow(pos[1] - mdvrp->customers[c].y, 2)));
            load += mdvrp->customers[c].demand;
            pos[0] = mdvrp->customers[c].x;
            pos[1] = mdvrp->customers[c].y;
        }
        if (load > mdvrp->depots[depot].maxLoad ||
                mdvrp->depots[depot].maxRoute != 0 && dist>mdvrp->depots[depot].maxRoute){
            return 0;
        }
    }
    dist += sqrt((pow(pos[0] - mdvrp->depots[depot].x, 2) + pow(pos[1] - mdvrp->depots[depot].y, 2)));
    if (mdvrp->depots[depot].maxRoute != 0 && dist>mdvrp->depots[depot].maxRoute){
        return 0;
    }
    return 1;
}


int validateSpecimen(MDVRP* mdvrp, Genotype* genotype){
    /*
    for (int i=0;i<genotype->m;i++){
        if (!validateTruck(mdvrp, i, genotype)){
            return 0;
        }
    }
    */
    //int cust[mdvrp->nCustomers];
    //only if broken
    /*
    for (int i=0;i<genotype->m*genotype->n;i++) {
        if (genotype->matrix[i] != -1) { cust[genotype->matrix[i]]++; }
    }
    for (int i=0;i<mdvrp->nCustomers;i++) {
        if (cust[i] != 1) {
            return 0;
        }
    }
    */
    int taken[mdvrp->nCustomers];
    for (int i=0;i<(genotype->n*genotype->m);i++){
        if (genotype->matrix[i] != -1){ taken[genotype->matrix[i]] = 1; }
    }
    for (int i=0;i<mdvrp->nCustomers;i++){
        if (taken[i] != 1){ return 0; }
    }
    return 1;
}


double calculateTruckDistance(MDVRP* mdvrp, int truck, Genotype* genotype){

    double dist = 0;
    int depot = truck/mdvrp->trucksPerDepot;
    int pos[2] = {mdvrp->depots[depot].x, mdvrp->depots[depot].y};
    for (int i=0;i<genotype->n;i++){
        if (genotype->matrix[truck*genotype->n+i] != -1) {
            int c = genotype->matrix[truck*genotype->n+i];
            dist += sqrt((pow(pos[0] - mdvrp->customers[c].x, 2) + pow(pos[1] - mdvrp->customers[c].y, 2)));
            pos[0] = mdvrp->customers[c].x;
            pos[1] = mdvrp->customers[c].y;
        }
    }
    dist += sqrt((pow(pos[0] - mdvrp->depots[depot].x, 2) + pow(pos[1] - mdvrp->depots[depot].y, 2)));
    return dist;
}


double calculateSpecimenDistance(MDVRP* mdvrp, Genotype* genotype){
    double ret = 0;
    for (int i=0;i<genotype->m;i++){
        ret += calculateTruckDistance(mdvrp, i, genotype);
    }
    return ret;
}


int calculateTruckLoad(MDVRP* mdvrp, int truck, Genotype* genotype){

    int load = 0;
    for (int i=0;i<genotype->n;i++){
        if (genotype->matrix[truck*genotype->n+i] != -1) {
            int c = genotype->matrix[truck*genotype->n+i];
            load += mdvrp->customers[c].demand;
        }
    }
    return load;
}


int calculateSpecimenLoad(MDVRP* mdvrp, Genotype* genotype){
    int ret = 0;
    for (int i=0;i<genotype->m;i++){
        ret += calculateTruckLoad(mdvrp, i, genotype);
    }
    return ret;
}


void calculateFitness(MDVRP* mdvrp, Genotype** population, int popSize, double* fitness){

    int depot;
    int load;
    double dist;

    for (int i=0;i<popSize;i++){
        fitness[i] = 0;
        for (int j=0;j<population[0]->m;j++){
            depot = j / mdvrp->trucksPerDepot;
            dist = calculateTruckDistance(mdvrp, j, population[i]);
            if (mdvrp->depots[depot].maxRoute != 0 && dist > mdvrp->depots[depot].maxRoute){
                dist *= dist;
            }
            load = calculateTruckLoad(mdvrp, j, population[i]);
            if (load > mdvrp->depots[depot].maxLoad){
                //printf("%d\n", ((load - mdvrp->depots[depot].maxLoad) + 1)*1000);
                dist += ((load - mdvrp->depots[depot].maxLoad) + 1)*10;
            }
            fitness[i] += dist;
        }
        //fitness[i] = calculateSpecimenDistance(mdvrp, population[i]);
    }

}


int parentOrderComp(const void* p1, const void* p2){
    double* pp1 = ((double*) p1);
    double* pp2 = ((double*) p2);

    if (pp1[1] > pp2[1]) return 1;
    if (pp1[1] < pp2[1]) return -1;
    return 0;
}


void crossoverSelection(int popSize, double* fitness, int* parents) {

    double fitnessSum = 0;
    for (int i = 0; i < popSize; i++) { fitnessSum += fitness[i]; }

    double p1[popSize * 2];
    double p2[popSize * 2];

    // assign a ticket for all parent slots
    int intFitSum = (int) floor(fitnessSum);
    for (int i = 0; i < popSize; i++) {
        p1[i * 2] = i;
        p1[i * 2 + 1] = getRandInt() % intFitSum;
        p2[i * 2] = i;
        p2[i * 2 + 1] = getRandInt() % intFitSum;
    }

    // sort tickets by ascending order
    qsort(p1, (size_t) popSize, sizeof(double) * 2, parentOrderComp);
    qsort(p2, (size_t) popSize, sizeof(double) * 2, parentOrderComp);

    // ticked counter
    int ip1 = 0;
    int ip2 = 0;
    // ticket limit reached
    int p1Done = 0;
    int p2Done = 0;
    // integrate fitness
    double fitAcc = 0;
    for (int i = 0; i < popSize; i++) {
        // return if all tickets are taken
        if (p1Done && p2Done) { return; }
        fitAcc += fitness[i];
        while (1) {
            // give tickets to current specimen
            int iip1 = ip1, iip2 = ip2;
            if (p1[ip1 * 2 + 1] <= fitAcc && !p1Done) {

                parents[(int) p1[ip1 * 2] * 2] = i;
                ip1++;
                if (ip1 == popSize) {
                    p1Done = 1;
                }
            }
            if (p2[ip2 * 2 + 1] <= fitAcc && !p2Done) {
                parents[(int) p2[ip2 * 2] * 2 + 1] = i;
                ip2++;
                if (ip2 == popSize) {
                    p2Done = 1;
                }
            }
            if (iip1 == ip1 && iip2 == ip2) {
                break;
            }
        }
    }
}


void nextGeneration(MDVRP* mdvrp, int elitism, double mutationRate, int doCrossover, int popSize, double* fitness, Genotype** population){

    Genotype** tmpPop = malloc(sizeof(Genotype*)*popSize);

    // index sort fitness
    double fitSortIxd[popSize*2];
    for (int i=0;i<popSize;i++){
        fitSortIxd[i*2] = i;
        fitSortIxd[i*2+1] = fitness[i];
    }

    // set off elites
    qsort(fitSortIxd, (size_t)popSize, sizeof(double)*2, parentOrderComp);
    for (int i=0;i<elitism;i++){
        tmpPop[i] = cloneGenotype(population[(int)fitSortIxd[i*2]]);
    }

    int parents[popSize*2];
    crossoverSelection(popSize, fitness, parents);
    if (normalRand()<.5){
        for (int i=0;i<popSize-elitism;i++){
            tmpPop[i+elitism] = crossover(mdvrp, population[parents[i*2]], population[parents[i*2+1]]);
            if(!validateSpecimen(mdvrp, tmpPop[i+elitism])) {
                i--;
            }
        };
    } else {
        for (int i=0;i<popSize-elitism;i++){
            tmpPop[i+elitism] = cloneGenotype(population[parents[i*2]]);
        }
    }

    mutatePopulation(mdvrp, mutationRate, popSize-elitism, &tmpPop[elitism]);
    destroyPopulation(popSize, population);
    for (int i=0;i<popSize;i++){
        population[i] = tmpPop[i];
    }

    // free up
    free(tmpPop);
}


void mutatePopulation(MDVRP* mdvrp, double mutationRate, int popSize, Genotype** population){
    for (int i=0;i<popSize;i++){
        for (int j=0;j<mdvrp->nCustomers;j++) {
            if (normalRand() < mutationRate) {
                switch (getRandInt() % 1) {
                    case 0:
                        swapMutate(mdvrp, population[i]);
                        break;
                    case 1:
                        shuffleSegmentMutate(mdvrp, population[i]);
                        break;
                    case 2:
                        swapTruckMutate(mdvrp, population[i]);
                        break;
                    case 3:
                        swapInRouteMutate(mdvrp, population[i]);
                        break;
                }
            }
        }
    }
}

void swapMutate(MDVRP *mdvrp, Genotype *specimen){
    while (1) {
        int segLen = 1;//(getRandInt() % 3)+1;
        int from = getRandInt() % (specimen->m*specimen->n-segLen);
        int to = getRandInt() % (specimen->m*specimen->n-segLen);
        int tmpTo[segLen];
        int tmpFrom[segLen];
        for (int j=0;j<segLen;j++){
            tmpFrom[j] = specimen->matrix[from+j];
            tmpTo[j] = specimen->matrix[to+j];
            specimen->matrix[from+j] = specimen->matrix[to+j];
        }
        // 50/50 chance to reverse segment
        if (normalRand() >= 1){
            for (int j=0;j<segLen;j++){
                specimen->matrix[to+j] = tmpFrom[j];
            }
        } else {
            for (int j=0;j<segLen;j++){
                specimen->matrix[to+j] = tmpFrom[segLen-1-j];
            }
        }
        // retry if not valid
        if (!validateSpecimen(mdvrp, specimen)){
            for (int j=0;j<segLen;j++){
                specimen->matrix[to+j] = tmpTo[j];
                specimen->matrix[from+j] = tmpFrom[j];
            }
        } else {
            break;
        }

    }
}


void shuffleSegmentMutate(MDVRP *mdvrp, Genotype *specimen){
    while (1) {
        int segLen = (getRandInt() % 5) + 2;
        int from = getRandInt() % (specimen->m * specimen->n - segLen);
        int tmp[segLen];
        for (int i = 0; i < segLen; i++) {
            tmp[i] = specimen->matrix[i + from];
        }

        qsort(&specimen->matrix[from], (size_t) segLen, sizeof(int), randComp);
        if (!validateSpecimen(mdvrp, specimen)) {
            for (int i = 0; i < segLen; i++) {
                specimen->matrix[i+from] = tmp[i];
            }
        } else {
            break;
        }
    }
}


void swapTruckMutate(MDVRP *mdvrp, Genotype *specimen){

    int t1 = getRandInt() % specimen->m;
    int t2 = getRandInt() % specimen->m;
    int tmp;

    for (int i=0;i<specimen->n;i++){
        tmp = specimen->matrix[t1*specimen->n+i];
        specimen->matrix[t1*specimen->n+i] = specimen->matrix[t2*specimen->n+i];
        specimen->matrix[t2*specimen->n+i] = tmp;
    }

}


void swapInRouteMutate(MDVRP *mdvrp, Genotype *specimen){

    int truck = (getRandInt() % specimen->m)*specimen->n;
    int from = getRandInt() % (specimen->n-2);
    int segLen = 1; //getRandInt() % specimen->n-segLen-1;
    int to = (getRandInt() % (specimen->n-from-segLen))+segLen+from;


    for (int i=0;i<segLen;i++){
        int tmp = specimen->matrix[truck+to];
        specimen->matrix[truck+to] = specimen->matrix[truck+from];
        specimen->matrix[truck+from] = tmp;
    }


}



void destroyPopulation(int popSize, Genotype** population){
    for (int i=0;i<popSize;i++){
        if (population[i] != NULL) {
            destroySpecimen(population[i]);
        }
    }
}


void destroySpecimen(Genotype* specimen){

    free(specimen->matrix);
    free(specimen);

}


Genotype* cloneGenotype(Genotype* specimen){

    Genotype* ret = malloc(sizeof(Genotype));
    ret->m = specimen->m;
    ret->n = specimen->n;

    size_t size = sizeof(int)*ret->m*ret->n;
    ret->matrix = malloc(size);
    memcpy(ret->matrix, specimen->matrix, size);

    return ret;
}


void printSpecimen(Genotype* specimen){
    for (int i=0;i<specimen->m;i++){
        for (int j=0;j<specimen->n;j++){
            printf("%d\t", specimen->matrix[i*specimen->n+j]);
        }
        printf("\n");
    }
}

Genotype* crossover(MDVRP* mdvrp, Genotype* p1, Genotype* p2){
    Genotype* child = initGenotype(mdvrp);

    int map[mdvrp->nCustomers*2];
    for (int i=0;i<(p1->m*p1->n);i++){
        if (p1->matrix[i] != -1){
            map[p1->matrix[i]*2] = i;
        }
        if (p2->matrix[i] != -1){
            map[p2->matrix[i]*2+1] = i;
        }
    }

    int idx;
    int parts[4] = {mdvrp->nCustomers/4, 2*mdvrp->nCustomers/4, 3*mdvrp->nCustomers/4, mdvrp->nCustomers};
    for (int i=0;i<mdvrp->nCustomers;i++){
        if (i < parts[0] || (i > parts[1] && i < parts[2])) {
            idx = map[i * 2];
        } else { idx = map[i * 2+1]; }
        if(child->matrix[idx] == -1) { child->matrix[idx] = i; }
        else {
            int j=1;
            while (1) {
                //printf("%d\n", j);
                if ((idx+j) < (p1->m*p1->n)) {
                    if (child->matrix[idx + j] == -1) {
                        child->matrix[idx + j] = i;
                        break;
                    }
                } else if((idx-j) >= 0) {
                    if (child->matrix[idx - j] == -1) {
                        child->matrix[idx - j] = i;
                        break;
                    }
                }
                j++;
            }
        }

    }
    return child;

}

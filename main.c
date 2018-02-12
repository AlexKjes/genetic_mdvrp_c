#include <stdio.h>
#include "mdvrp.h"
#include "genetics.h"
#include "output.h"
#include "util.h"
#include <gsl/gsl_statistics.h>
#include <limits.h>

double updateMutationRate(double* fitness, double* prevFitness, int popSize, double currentMr){

    double yolo = 0;
    for (int i=0;i<popSize;i++){
        yolo += fitness[i] > prevFitness[i];
    }
    yolo/=popSize;
    //printf("%lf\n", yolo);
    if (yolo < 1/2){
        return currentMr*1.00001;
    }
    return currentMr*0.99999;

}



int main() {





    initRand();
    // ./Testing Data/Data Files/p01
    MDVRP* mdvrp = loadProblem("../Testing Data/Data Files/p01");

    int popSize = 50;
    Genotype** population = generateRandomPopulation(mdvrp, popSize);
    double fitness[2][popSize];
    int genNum = 0;
    double globalFittest = INT_MAX;
    double mutationRate = .01;

    initGNUPlot(mdvrp->nDepots*mdvrp->trucksPerDepot);
    drawMDVRP(mdvrp);
    while (1){

       calculateFitness(mdvrp, population, popSize, fitness[genNum%2]);
       mutationRate = normalRand()/10; //updateMutationRate(fitness[genNum%2], fitness[(genNum+1)%2], popSize, mutationRate);
       nextGeneration(mdvrp, 10, mutationRate, 1, popSize, fitness[genNum%2], population);


       double fittest=INT_MAX;
       for (int i=0;i<popSize;i++) {

           if (fitness[genNum%2][i] < fittest) fittest = fitness[genNum%2][i];
       }

       if (fittest<globalFittest) {

           double meanfit=0;
           for (int i=0;i<popSize;i++){ meanfit+=fitness[genNum%2][i]; }
           meanfit/=popSize;

           globalFittest = fittest;
           printf("Gen: %d\tMutationRate: %lf\n", genNum, mutationRate);
           printf("Fittesrt: %d\tMeanFitness: %lf\n", (int) fittest, meanfit);
           printSpecs(mdvrp, population[0]);

           drawSpecimen(mdvrp, population[0]);
           printf("---------------------------------------------\n");

       }
       genNum++;
       if (fittest==0){
           break;
       }
    }


    return 0;
}



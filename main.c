#include <stdio.h>
#include "mdvrp.h"
#include "genetics.h"
#include "output.h"
#include "util.h"
#include <limits.h>

double updateMutationRate(double* fitness, double* prevFitness, int popSize, double currentMr){

    double yolo = 0;
    for (int i=0;i<popSize;i++){
        yolo += fitness[i];
    }
    yolo/=popSize;

    if (yolo*0.80 < fitness[0]){
        return currentMr*1.1;
    }
    return currentMr*0.99;

}


const int POPULATION_SIZE = 50;
const int ELITISM         =  1;
const int MC              = 12;

/// Id of plot files
const int PLOT_NUMBER = 2;


int main() {





    initRand();
    // ./Testing Data/Data Files/p01
    MDVRP* mdvrp = loadProblem("../Testing Data/Data Files/p23");

    int popSize = POPULATION_SIZE;
    Genotype** population = generateRandomPopulation(mdvrp, popSize);
    double fitness[2][popSize];
    int genNum = 0;
    double globalFittest = INT_MAX;
    double mutationRate = .01;


    initGNUPlot(mdvrp->nDepots*mdvrp->trucksPerDepot, PLOT_NUMBER);
    drawMDVRP(mdvrp);
    while (1){

       calculateFitness(mdvrp, population, popSize, fitness[genNum%2]);
       nextGeneration(mdvrp, ELITISM, mutationRate, 1, popSize, fitness[genNum%2], population);
        if ((genNum%15)==1)
            mutationRate = normalRand()/MC; //updateMutationRate(fitness[genNum%2], fitness[(genNum+1)%2], popSize, mutationRate);



       double fittest=INT_MAX;
       for (int i=0;i<popSize;i++) {

           if (fitness[genNum%2][i] < fittest) fittest = fitness[genNum%2][i];
       }

       if (fittest<globalFittest /*|| (genNum%10000)==0*/) {


           double meanfit=0;
           for (int i=0;i<popSize;i++){ meanfit+=fitness[genNum%2][i]; }
           meanfit/=popSize;

           globalFittest = fittest;
           printf("Gen: %d\tMutationRate: %lf\n", genNum, mutationRate);
           printf("Fittesrt: %d\tMeanFitness: %lf\n", (int) fitness[genNum%2][0], meanfit);
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



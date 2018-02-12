#include <stdio.h>
#include "mdvrp.h"
#include "genetics.h"
#include "output.h"
#include "util.h"
#include <gsl/gsl_statistics.h>


double mutationRate(double* fitness, int popSize, double current_mr){

    double var = gsl_stats_long_double_variance(fitness, sizeof(double), popSize);

}



int main() {

    initRand();
    // ./Testing Data/Data Files/p01
    MDVRP* mdvrp = loadProblem("../Testing Data/Data Files/p02");
    int popSize = 100;
    Genotype** population = generateRandomPopulation(mdvrp, popSize);
    double fitness[popSize];
    int genNum = 0;
    double globalFittest = 999999999;

    initGNUPlot(mdvrp->nDepots*mdvrp->trucksPerDepot);
    drawMDVRP(mdvrp);
    while (1){
        calculateFitness(mdvrp, population, popSize, fitness);

        nextGeneration(mdvrp, 20, 0.02, 1, popSize, fitness, population);


        double fittest=9999999999;
        for (int i=0;i<popSize;i++) {
            if (fitness[i] < fittest) fittest = fitness[i];
        }

        if (fittest<globalFittest) {

            globalFittest = fittest;
            printf("Gen: %d\nFittest: %d\n", genNum, (int) fittest);
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



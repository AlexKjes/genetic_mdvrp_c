#include <stdio.h>
#include "mdvrp.h"
#include "genetics.h"
#include "output.h"

int main() {

    // ./Testing Data/Data Files/p01
    MDVRP* mdvrp = loadProblem("../Testing Data/Data Files/p01");
    int popSize = 10;
    Genotype** population = generateRandomPopulation(mdvrp, popSize);
    /*
    double fitness[popSize];
    calculateFitness(mdvrp, population, popSize, fitness);

    for (int i=0;i<population[0]->m;i++){
        for (int j=0;j<population[0]->n;j++){
            printf("%d\t", population[0]->matrix[i*population[0]->n+j]);
        }
        printf("\n");
    }

    for (int i=0;i<40;i++){
        printf("=");
    }
    printf("\n");

    mutateSpecimen(mdvrp, 0.3, population[0]);

    for (int i=0;i<population[0]->m;i++){
        for (int j=0;j<population[0]->n;j++){
            printf("%d\t", population[0]->matrix[i*population[0]->n+j]);
        }
        printf("\n");
    }

    //nextGeneration(mdvrp, 1, 0.1, 0, popSize, fitness, population);
    */

    initGNUPlotPipe();
    drawMDVRP(mdvrp);
    drawSpecimen(mdvrp, population[0]);
    drawSpecimen(mdvrp, population[5]);




    return 0;
}



//
// Created by alex on 2/7/18.
//

#include <stdio.h>
#include "output.h"
#include "mdvrp.h"


FILE* gpp;
FILE* routesData;
FILE* mdvrpData;


const char* rfp = "/tmp/routes.data";
const char* mfp = "/tmp/mdvrp.data";


void initGNUPlotPipe(){

    gpp = popen("gnuplot -persistent", "w");
    fprintf(gpp, "set title \"MDVRP\"\nset terminal wxt size 800, 800\n"
            "set output 'single_point.svg'");
    fprintf(gpp, "set border linewidth 1.5\n"
            "set pointsize 1.5\n"
            "set style line 1 lc rgb '#0060ad' pt 5   # star\n"
            "set style line 2 lc rgb '#dd0000' pt 7   # circle\n"
            "set style line 3 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5\n");
}

void drawMDVRP(MDVRP* mdvrp){

    mdvrpData = fopen(mfp, "w");
    if (mdvrpData == NULL) { exit(EXIT_FAILURE); }
    fprintf(gpp, "plot '%s' index 0 with points ls 1 title 'Depots', '' index 1 with points ls 2 title 'Customers'\n", mfp);
    for (int i=0;i<mdvrp->nDepots;i++){
        fprintf(mdvrpData, "%d %d\n", mdvrp->depots[i].x, mdvrp->depots[i].y);
    }
    fprintf(mdvrpData, "\n\n");
    for (int i=0;i<mdvrp->nCustomers;i++){
        fprintf(mdvrpData, "%d %d\n", mdvrp->customers[i].x, mdvrp->customers[i].y);
    }
    fclose(mdvrpData);
    //fprintf(gpp, "plot '%s' index 0 with points ls 1, index 1 with points ls 2\n", mfp);
}


void drawSpecimen(MDVRP* mdvrp, Genotype* genotype){


    routesData = fopen(rfp, "w");
    if (routesData == NULL) { exit(EXIT_FAILURE); }
    fprintf(gpp, "replot '%s' with lines\n", rfp);
    for (int i=0;i<genotype->m;i++){
        for (int j=0;j<genotype->n;j++){
            int idx = i*genotype->n+j;
            if (genotype->matrix[idx] != -1){
                int x = mdvrp->customers[genotype->matrix[idx]].x;
                int y = mdvrp->customers[genotype->matrix[idx]].y;
                fprintf(routesData, "%d\t%d\n", x, y);
            }
        }
        fprintf(routesData, "\n");
    }
    fclose(routesData);

    printf("WTF");
}
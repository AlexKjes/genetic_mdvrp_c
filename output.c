//
// Created by alex on 2/7/18.
//

#include <stdio.h>
#include "output.h"
#include "mdvrp.h"
#include "genetics.h"


FILE* gpp;
FILE* routesData;



const char* rfp = "/tmp/routes.data";
const char* mfp = "/tmp/mdvrp.data";


void initGNUPlot(int genomeLen){

    char* confPath = "/tmp/plot.conf";

    fclose(fopen(rfp, "w"));
    routesData = fopen(rfp, "w");
    for (int i=0;i<genomeLen;i++){ fprintf(routesData, "0\t0\n\n"); }
    fclose(routesData);

    FILE* plotConf = fopen(confPath, "w");

    fprintf(plotConf, "set title \"MDVRP\"\nset terminal wxt size 900, 900\n"
            "set output 'single_point.svg'\n");
    fprintf(plotConf, "set border linewidth 1.5\n"
            "set pointsize 1.5\n"
            "set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 5 ps 1.5\n"
            "set style line 2 lc rgb '#dd0000' lt 1 lw 2 pt 7 ps 1.5\n"
            "set style line 3 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5\n"
            "set style line 4 lc rgb '#00dd00' lt 1 lw 2 pt 7 ps 1.5\n"
            "set style line 5 lc rgb '#eeee00' lt 1 lw 2 pt 7 ps 1.5\n"
            "set style line 6 lc rgb '#00ddee' lt 1 lw 2 pt 7 ps 1.5\n"
            "set style line 7 lc rgb '#dd00ee' lt 1 lw 2 pt 7 ps 1.5\n"
            "set style line 8 lc rgb '#660066' lt 1 lw 2 pt 7 ps 1.5\n");

    fprintf(plotConf, "plot '%s' index 0 with points ls 1 title 'Depots', "
            "'' index 1 with points ls 2 title 'Customers', "
            "'%s' index 0 with lines ls 3 notitle, \\\n", mfp, rfp);
    for (int i=1;i<genomeLen;i++){
        fprintf(plotConf, "'' index %d with lines ls %d notitle, \\\n", i, ((i+4)%8)+1);
    }
    fprintf(plotConf, "\npause 1\nreread\n");
    fclose(plotConf);
    gpp = popen("gnuplot -c /tmp/plot.conf", "w");
    //fprintf(gpp, "load \"%s\"", confPath);

}

void drawMDVRP(MDVRP* mdvrp){

    FILE* mdvrpData;

    mdvrpData = fopen(mfp, "w");
    if (mdvrpData == NULL) { exit(EXIT_FAILURE); }
    for (int i=0;i<mdvrp->nDepots;i++){
        fprintf(mdvrpData, "%d %d\n", mdvrp->depots[i].x, mdvrp->depots[i].y);
    }
    fprintf(mdvrpData, "\n\n");
    for (int i=0;i<mdvrp->nCustomers;i++){
        fprintf(mdvrpData, "%d %d\n", mdvrp->customers[i].x, mdvrp->customers[i].y);
    }
    fclose(mdvrpData);

}


void drawSpecimen(MDVRP* mdvrp, Genotype* genotype){

    routesData = fopen(rfp, "w");
    if (routesData == NULL) { exit(EXIT_FAILURE); }
    for (int i=0;i<genotype->m;i++){
        int d = i/mdvrp->trucksPerDepot;
        fprintf(routesData, "%d\t%d\n", mdvrp->depots[d].x, mdvrp->depots[d].y);
        for (int j=0;j<genotype->n;j++){
            int idx = i*genotype->n+j;
            if (genotype->matrix[idx] != -1){
                int x = mdvrp->customers[genotype->matrix[idx]].x;
                int y = mdvrp->customers[genotype->matrix[idx]].y;
                fprintf(routesData, "%d\t%d\n", x, y);
            }
        }
        fprintf(routesData, "%d\t%d\n", mdvrp->depots[d].x, mdvrp->depots[d].y);
        fprintf(routesData, "\n\n");
    }
    fclose(routesData);

}


void printSpecs(MDVRP* mdvrp, Genotype* specimen){

    int depot;
    for (int i=0;i<specimen->m;i++){
        depot = i / mdvrp->trucksPerDepot;

        printf("%d\t%d\t", i, depot);
        printf("%d\t", calculateTruckLoad(mdvrp, i, specimen));
        printf("%lf\t", calculateTruckDistance(mdvrp, i, specimen));
        for (int j=0;j<specimen->n;j++){
            if (specimen->matrix[i*specimen->n+j] != -1)
                printf("%d\t", specimen->matrix[i*specimen->n+j]);
        }
        printf("\n");
    }
}
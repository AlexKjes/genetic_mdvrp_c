//
// Created by alex on 2/7/18.
//

#include <stdio.h>
#include "output.h"
#include "mdvrp.h"


FILE* gpp;


void initGNUPlotPipe(){

    gpp = popen("gnuplot -persistent", "w");
    fprintf(gpp, "set title \"MDVRP\"\nset terminal wxt size 800, 800\n"
            "set output 'single_point.svg'");
    fprintf(gpp, "set border linewidth 1.5\n"
            "set pointsize 1.5\n"
            "set style line 1 lc rgb '#0060ad' pt 5   # square\n"
            "set style line 2 lc rgb '#dd0000' pt 7   # circle\n"
            "set style line 3 lc rgb '#0060ad' pt 9   # triangle\n");

}

void drawMDVRP(MDVRP* mdvrp){

    fprintf(gpp, "plot '-' w p ls 1, '-' w p ls 2\n");
    for (int i=0;i<mdvrp->nDepots;i++){
        fprintf(gpp, "%d %d\n", mdvrp->depots[i].x, mdvrp->depots[i].y);
    }
    fprintf(gpp, "e\n");
    for (int i=0;i<mdvrp->nCustomers;i++){
        fprintf(gpp, "%d %d\n", mdvrp->customers[i].x, mdvrp->customers[i].y);
    }

}
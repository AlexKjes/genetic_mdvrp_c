//
// Created by alex on 2/5/18.
//

#include "mdvrp.h"

/**
 * Takes a string of ints, splits it and parses to int, string needs to end in linebreak
 * @param delimiter The delimiter to split the string on
 * @param line
 * @param size number in ints in line
 * @param buffer return buffer
 */

void parseLine(char delimiter, const char* line, int* size, int* buffer){

    int nInts = 0;
    int intLen = 0;
    char strBuff[20];

    for (int i=0;1;i++){
        if (line[i] != delimiter && line[i] != '\n'){
            strBuff[intLen] = line[i];
            intLen++;
        }else if (intLen != 0){
            strBuff[intLen] = '\n';
            buffer[nInts] = atoi(strBuff);
            intLen = 0;
            nInts++;
        }
        if (line[i] == '\n'){
            break;
        }
    }
    *size = nInts;

}

/// allocate a mdvrp instance
MDVRP* initMDVRP(int nCustomers, int nDepots){

    MDVRP* mdvrp = malloc(sizeof(MDVRP));

    mdvrp->nCustomers = nCustomers;
    mdvrp->nDepots = nDepots;

    mdvrp->customers = malloc(nCustomers * sizeof(Customer));
    mdvrp->depots = malloc(nDepots * sizeof(Depot));

}

/// load MDVRP problem from file
MDVRP* loadProblem(char* filePath){

    FILE* fp;
    char* line;
    size_t len = 0;
    ssize_t read;

    MDVRP* mdvrp;
    int intBuff[20];
    int intLen;
    fp = fopen(filePath, "r");


    if (fp == NULL){
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while ((read = getline(&line, &len, fp)) != -1){
        parseLine(' ', line, &intLen, intBuff);
        // read header
        if(i == 0){
            mdvrp = initMDVRP(intBuff[1], intBuff[2]);
            mdvrp->trucksPerDepot = intBuff[0];
            // read depots part 1
        } else if (i<(mdvrp->nDepots+1)){
            mdvrp->depots[i-1].maxRoute = intBuff[0];
            mdvrp->depots[i-1].maxLoad = intBuff[1];
            // read customer data
        } else if (i<mdvrp->nDepots+mdvrp->nCustomers+1){
            mdvrp->customers[intBuff[0]-1].x = intBuff[1];
            mdvrp->customers[intBuff[0]-1].y = intBuff[2];
            mdvrp->customers[intBuff[0]-1].demand = intBuff[4];
            // read depots part 2
        } else if (i<mdvrp->nDepots*2+mdvrp->nCustomers+1){
            int id = i-mdvrp->nCustomers-mdvrp->nDepots-1;
            mdvrp->depots[id].x = intBuff[1];
            mdvrp->depots[id].y = intBuff[2];
        }
        i++;
    }

    fclose(fp);
    if (line){
        free(line);
    }

    return mdvrp;

}



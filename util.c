//
// Created by alex on 2/6/18.
//

#include "util.h"
#include <gsl/gsl_rng.h>
#include <time.h>


gsl_rng* r;



void initRand(){

    time_t t;
    time(&t);
    r = gsl_rng_alloc(gsl_rng_default);
    gsl_rng_set(r, (unsigned long int)t);

}


int randComp(const void* i1, const void* i2){
    if (getRandInt() < getRandInt()) return 1;
    else return -1;
}


void randIntArr(int low, int high, int size, int* buffer){
    for (int i=0;i<size;i++){
        buffer[i] = getRandInt() % (high-low) + high;
    }
}


int getRandInt(){
    return gsl_rng_get(r)/2;
}

double normalRand(){
    return gsl_rng_uniform(r);
}
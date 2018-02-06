//
// Created by alex on 2/6/18.
//

#include "util.h"
#include <gsl/gsl_rng.h>


gsl_rng* r;


int randComp(const void* i1, const void* i2){
    if (getRandInt() < getRandInt()) return 1;
    else return -1;
}


void randIntArr(int low, int high, int size, int* buffer){
    for (int i=0;i<size;i++){
        buffer[i] = rand() % (high-low) + high;
    }
}


int getRandInt(){
    if (r == NULL){
        r = gsl_rng_alloc(gsl_rng_default);
    }
    return gsl_rng_get(r);
}
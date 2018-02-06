#include <stdio.h>
#include "mdvrp.h"
#include "genetics.h"

int main() {
    // ./Testing Data/Data Files/p01
    MDVRP* mdvrp = loadProblem("../Testing Data/Data Files/p01");
    makeRandomSpecimen(mdvrp);

    return 0;
}



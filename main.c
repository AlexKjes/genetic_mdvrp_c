#include <stdio.h>
#include "mdvrp.h"
#include "genetics.h"

int main() {
    // ./Testing Data/Data Files/p01
    MDVRP* mdvrp = loadProblem("../Testing Data/Data Files/p01");
    routeMax(mdvrp);

    return 0;
}



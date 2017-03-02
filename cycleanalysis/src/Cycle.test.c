#include <stdio.h>
#include <stdlib.h>
#include "Cycle.h"

int main(int argc, char *argv[]){
    Cycle * cycle = createCycle();

    //Append and print test
    appendNode("S1", cycle);
    appendNode("S2", cycle);
    appendNode("S3", cycle);

    printf("Expecting cycle to print S1,S2,S3...\n");
    printCycle(cycle);

    //Length Test
    printf("Expecting cycle to be of length 3...\n");
    int length = getCycleLength(cycle);
    printf("%d\n\n", length);

    //Freeing Test
    freeCycle(cycle);
    printf("Expecting segmentation fault after freeing cycle...\n");
    printCycle(cycle);

    return (0);
}
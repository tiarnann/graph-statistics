#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Cycle.h"
#include "CycleList.h"

int main(int argc, char *argv[]){
    CycleList * list = createCycleList();
    Cycle * cycle = createCycle();

    appendCycle(cycle, list);

    //Append and print test
    appendNode("S1", cycle);
    appendNode("S2", cycle);
    appendNode("S3", cycle);

	printf("Expecting cycle to print S1,S2,S3---\n");
    printCycleList(list);

    //Length Test
    printf("Expecting cycle to be of length 1...\n");
    int length = list -> size;
    printf("%d\n\n", length);

    cycle = createCycle();
    appendNode("S4", cycle);
    appendNode("S5", cycle);
    appendNode("S6", cycle);
    appendCycle(cycle, list);

   	printf("Expecting cycle to print S1,S2,S3---S4,S5,S6\n");
    printCycleList(list);
    
    //Length Test
    printf("Expecting cycle to be of length 2...\n");
    length = list -> size;
    printf("%d\n\n", length);

    //Freeing Test
    freeCycleList(list);
    printf("Expecting segmentation fault after freeing cycle...\n");
    printCycleList(list);

    return (0);
}
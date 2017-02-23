#include <stdio.h>
#include <stdlib.h>
#include "SingleLinkedListTest.h"

int main (){
    struct idList * list1 = createIDList();

    appendID("S1", list1);
    printIDList(list1);

    appendID("S2", list1);
    printIDList(list1);

    appendID("S3", list1);
    printIDList(list1);
}

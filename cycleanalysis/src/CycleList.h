#ifndef CYCLE_H
#define CYCLE_H
#include <stdio.h>
#include <stdlib.h>
#include "Cycle.h"


//-----------------------------------------------------------------------------------------------
//LISTNODES & LISTS OF LISTNODES
//Structures
struct _CycleList{
	int size;
    Cycle *head;
    Cycle *tail;
} typedef CycleList;

//Constructors

CycleList *createCycleList(){
    CycleList *result = malloc(sizeof(CycleList));
    result -> head = NULL;
    result -> tail = NULL;
    result.size = 0;
    return result;
};

// Functions
void appendCycle (Cycle *cycle, CycleList *list){
	//append cycle to list
    if (list -> head == NULL){
		list -> head = cycle;
		list -> tail = cycle;
    }
	else {
		list -> tail -> next = cycle;
		list -> tail = cycle;
	}	
	list.size += 1;
}

void printCycleList(CycleList *list) {
	if(list == NULL) return;

	CycleList *current = list->head;

	while(current != NULL){
		printCycle(current);
		current = current-> next;
	}
}

void freeCycleList(CycleList *list)
{
	Cycle* current = cycle -> head;
	Cycle* next;
	while(current != NULL){
		next = current -> next;
		freeCycle(current);
		current = next; 
	}
	freeCycle(cycle);
} 

#endif CYCLE_H

#ifndef CYCLE_LIST_H
#define CYCLE_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include "Cycle.h"

typedef struct _CycleListNode{
    struct _Cycle *list;
    struct _CycleListNode *next;
} CycleListNode;

typedef struct _CycleList{
	int size;
    struct _CycleListNode *head;
    struct _CycleListNode *tail;
} CycleList;


CycleListNode *createCycleListNode(){
	CycleListNode *result = malloc(sizeof(CycleListNode));
    result -> list = NULL;
    result -> next = NULL;
    return result;
}

/**
 * [createCycleList creates new empty cycle list]
 * @return {CycleList} [empty cycle list]
 */
CycleList *createCycleList(){
    CycleList *result = malloc(sizeof(CycleList));
    result -> head = NULL;
    result -> tail = NULL;
    result -> size = 0;
    return result;
};

/**
 * [appendCycle  [appends cycle to cycle list]
 * @param {Cycle} cycle 	[cycle to add to list]
 * @param {CycleList} list  [list to add cycle to]
 */
void appendCycle(Cycle *cycle, CycleList *list){
	CycleListNode *result= createCycleListNode();
	result -> list = cycle;

	//append cycle to list
    if (list -> head == NULL){
		list -> head = result;
		list -> tail = result;
    }
	else {
		list -> tail -> next = result;
		list -> tail = result;
	}
	list -> size += 1;
}

/**
 * [printCycleList prints all cycles]
 * @param list [list of cycles]
 */
void printCycleList(CycleList *list) {
	if(list == NULL) return;

	CycleListNode *current = list -> head;

	int i=1;
	while(current != NULL){
	  printf("\n[%d]\n", i++);
		printCycle(current -> list);
		current = current -> next;
	}
}

/**
 * [freeCycleList frees CycleList and all Cycles contained within that list]
 * @param list [description]
 */
void freeCycleList(CycleList *list)
{
	CycleListNode* current = list -> head;
	CycleListNode* next;
	while(current != NULL){
		next = current -> next;
		freeCycle(current -> list);
		free(current);
		current = next;
	}
	free(list);
}

#endif

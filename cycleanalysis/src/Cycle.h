#ifndef CYCLE_H
#define CYCLE_H
#include <stdio.h>
#include <stdlib.h>

//ID LISTS & NODES
// Structures
typedef struct _Node {
    char *id;
    struct _Node *next;
} Node;

typedef struct _Cycle {
	int size;
    Node *head;
    Node *tail;
} Cycle;

/**
 * [createNode creates new Node with passed id]
 * @param  id 		[id to assign to node]
 * @return {Node}   [newly created node]
 */
Node *createNode(char* id){
    Node* result = malloc(sizeof(Node));
    result -> id = id;
    result -> next = NULL;
    return result;
}


/**
 * [createCycle creates and initialises new node cycle]
 * @return {Cycle} [newly created cycle]
 */
Cycle *createCycle(){
    Cycle *result = malloc(sizeof(Cycle));
	result -> size = 0;
	result -> head = NULL;
	result -> tail = NULL;
    return result;
}

/**
 * [appendNode appends node to cycle]
 * @param {char *}	id 		[id of new node to append]
 * @param {Cycle} 	cycle 	[cycle of nodes]
 */
void appendNode(char *id, Cycle *cycle){
	//set up new node
	Node *newNode = createNode(id);

	//append node to cycle
    if (cycle -> head == NULL){
		cycle -> head = newNode;
		cycle -> tail = newNode;
    }
	else {
		cycle -> tail -> next = newNode;
		cycle -> tail = newNode;
	}
	cycle -> size += 1;
}

/**TODO
 * [removeNode removes node at certain index. this does not free the Node pointer!]
 * @return {int} [0 if node was successfully removed, -1 if not]
 */
int removeNode(){
	return -1;
}

/**
 * [printCycle prints ids of nodes within given cycle]
 * @param {Cycle} cycle [description]
 */
void printCycle(Cycle *cycle) {
	if(cycle == NULL) return;

	Node *current = cycle -> head;

	while(current != NULL){
		printf("%s\n", current -> id);
		current = current -> next;
	}
}

/**
 * [getCycleLength returns length of cycle]
 * @param  {Cycle} cycle [cycle of nodes]
 * @return {int}     	[length of cycle]
 */
int getCycleLength(Cycle *cycle) {
	if(cycle == NULL) return -1;
	return cycle -> size;
}

/**
 * [freeCycle frees cycle pointers and nodes within that cycle]
 * @param {Cycle} cycle [cycle of nodes]
 */
void freeCycle(Cycle *cycle) {
	Node* current = cycle->head;
	Node* next;
	while(current != NULL){
		next = current -> next;
		free(current);
		current = next; 
	}
	free(cycle);
}

#endif
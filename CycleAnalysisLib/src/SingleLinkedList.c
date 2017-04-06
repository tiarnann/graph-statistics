#include <stdio.h>
#include <stdlib.h>

//ID LISTS & NODES
// Structures
struct idNode {
    char *id;
    struct idNode *next;
};

struct idList {
	int size;
    struct idNode *head;
    struct idNode *tail;
};

// Constructors
struct idNode *createIDNode(char* id){
    struct idNode *result = malloc(sizeof(struct idNode));
    result -> id = id;
    result -> next = NULL;
    return result;
}

struct idList *createIDList() {
    struct idList *result = malloc(sizeof(struct idList));
	result -> size = 0;
	result -> head = NULL;
	result -> tail = NULL;
    return result;
}

//Functions
void appendID (char *id, struct idList *list){
	//set up new node
	struct idNode *newNode = createIDNode(id);

	//append node to list
    if (list -> head == NULL){
		list -> head = newNode;
		list -> tail = newNode;
    }
	else {
		list -> tail -> next = newNode;
		list -> tail = newNode;
	}
	list -> size++;
}

void printIDList(struct idList *list) {
	if(list == NULL) return;

	struct idNode *current = list->head;

	while(current != NULL){
		printf("%s\n", current->id);
		current = current-> next;
	}
}

/**
 * [getListLength returns length of the list or -1 if the list is undefined]
 */
int getListLength(struct idList *list) {
	if(list == NULL) return -1;
	struct idNode *current = list->head;

	int length = 0;
	while(current != NULL){
		length++;
		current = current-> next;
	}

	return length;
}

//-----------------------------------------------------------------------------------------------
//LISTNODES & LISTS OF LISTNODES
//Structures
struct listNode{
    struct idList *idList;
    struct listNode *next;
};

struct listOfListNodes{
	long totalSize;
    struct listNode *head;
    struct listNode *tail;
};

//Constructors

struct listNode *createListNode(struct idList *list){
    struct listNode *result = malloc(sizeof(struct listNode));
    result -> idList = list;
    result -> next = NULL;
    return result;
};

struct listOfListNodes *createListOfListNodes(){
    struct listOfListNodes *result = malloc(sizeof(struct listOfListNodes));
	result -> totalSize = 0;
	result -> head = NULL;
	result -> tail = NULL;
    return result;
};

// Functions
void appendList (struct idList *list,struct listOfListNodes *listOfLists){
	//set up new listNode
	struct listNode *newNode = createListNode(list);
	//append node to list
    if (listOfLists -> head == NULL){
		listOfLists -> head = newNode;
		listOfLists -> tail = newNode;
    }
	else {
		listOfLists -> tail -> next = newNode;
		listOfLists -> tail = newNode;
	}
	listOfLists -> totalSize++;
}

void printListOfListNodes(struct listOfListNodes *listOfLists) {
	if(listOfLists == NULL) return;

	struct listNode *current = listOfLists->head;

	while(current != NULL){
		printIDList(current->idList);
		current = current-> next;
	}
}

int main(int argc, char *argv[]){
    struct idList * list1 = createIDList();

    appendID("S1", list1);

    appendID("S2", list1);

    appendID("S3", list1);
    printIDList(list1);

    return (0);
}



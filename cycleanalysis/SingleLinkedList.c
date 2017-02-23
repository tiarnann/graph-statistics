#include<stdio.h>
#include<stdlib.h>

//ID LISTS & NODES
// Structures
struct idNode{
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
    return result;
}

struct idList *createIDList() {
    struct idList *result = malloc(sizeof(struct idList));
	result -> size = 0;
    return result;
}

//Functions
void appendID (char *id, struct idList *list){
	//set up new node
	struct idNode *newNode = malloc(sizeof(struct idNode));
	newNode = createIDNode(id);

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

struct listNode *createListNode(){
    struct listNode *result = malloc(sizeof(struct listNode));
    result -> idList = createIDList();
    return result;
};

struct listOfListNodes *createListOfListNodes(){
    struct listOfListNodes *result = malloc(sizeof(struct listOfListNodes));
	result -> totalSize = 0;
    return result;
};

// Functions
void appendList (struct listOfListNodes *listOfLists){
	//set up new listNode
	struct listNode *newNode = createListNode();
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

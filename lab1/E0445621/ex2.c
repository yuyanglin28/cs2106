#include <stdio.h>
#include <stdlib.h>
// stdlib includes malloc() and free()

// define instructions
#define PREVIOUS -1
#define NEXT 1
#define DELETE 0
#define INSERTSUBNODE 2 // New instruction compared to ex1.

// Node and SubNode declarations ( Do not Edit )
typedef struct SUBNODE
{
	int data;
	struct SUBNODE* nextSubNode;
} subNode;

typedef struct NODE
{
	int data;
	struct NODE* previousNode;
	struct NODE* nextNode;
	subNode* subNodeHead;
} node;

// Function prototypes
void insertNodeNext(int,int,node*); // TODO
void insertNodePrevious(int,int,node*); //TODO
void deleteNode(int,node*); // TODO
void deleteList(node*); // TODO
void insertSubNode(int,int,int,node*); // TODO
void deleteAllSubNodes(node*); // Optional TODO: will be useful in ex3.

void printList(node*); // Implemented for you
void printSubNodes(node*); // Implemented for you :Helper function for print list

// Start of main
int main()
{
	int position;
	int instruction;
	int subPosition;
	int value;

	// Declaration of the origin Node ( do not edit )
	node* originNode = (node*)malloc(sizeof(node));
	originNode->previousNode = originNode;
	originNode->nextNode = originNode;
	originNode->data = 0;
	originNode->subNodeHead = NULL;

	// input and processing has been done for you
	while(scanf("%i%i",&instruction,&position) == 2)
		//scanf() returns an int that corresponds to the number of values scanned.
	{
		if(instruction == DELETE)
		{
			deleteNode(position,originNode);
		}
		else if(instruction == INSERTSUBNODE)
		{
			scanf("%i%i",&subPosition,&value);
			insertSubNode(position,subPosition,value,originNode);
		}
		else if(instruction == NEXT)
		{
			scanf("%i",&value);
			insertNodeNext(position,value,originNode);
		}
		else if(instruction == PREVIOUS)
		{
			scanf("%i",&value);

			insertNodePrevious(position,value,originNode);
		}
	}

	printList(originNode);
	deleteList(originNode);

	printf("Circular List after delete\n");
	printList(originNode);

	free(originNode);
	return 0;
}


//Function Implementations
void insertSubNode(int position,int subPosition,int value,node* originNode)
{
	for (int i=0; i<position; i++){
		originNode = originNode->nextNode;
	}

	subNode* insert = (subNode*)malloc(sizeof(subNode));
	insert->data = value;
	insert->nextSubNode = NULL;

	if (originNode->subNodeHead == NULL){
		originNode->subNodeHead = insert;
	}else{
		subNode* subPre;
		subNode* subNext;
		if (subPosition == 0){
			subNext = originNode->subNodeHead;
			originNode->subNodeHead = insert;
			insert->nextSubNode = subNext;
		}else{
			subPre = originNode->subNodeHead;
		   for (int i=0; i<subPosition-1; i++){
				  subPre= subPre->nextSubNode;
		   }
		   subNext = subPre->nextSubNode;
		   subPre->nextSubNode = insert;
		   insert->nextSubNode = subNext;
	  }
	}

}

void insertNodePrevious(int position,int value, node* originNode)
{
	node* insertNode = (node*)malloc(sizeof(node));
	node* pre;
	node* next;
	node* ptr = originNode;

	if (position == 0){
		while (ptr->nextNode != originNode){
			ptr = ptr->nextNode;
		}
		pre = ptr;
		next = originNode;
	}else{
		pre = originNode;
		next = originNode->nextNode;
		for (int i=0; i<position-1; i++){
			next = next->nextNode;
			pre = pre->nextNode;
		}
	}

	insertNode->previousNode = pre;
	insertNode->nextNode = next;
	insertNode->data = value;
	pre->nextNode = insertNode;
	next->previousNode = insertNode;
}

void insertNodeNext(int position, int value, node* originNode)
{
	node* insertNode = (node*)malloc(sizeof(node));
	node* pre = originNode;
	node* next = originNode->nextNode;
	for (int i=0; i<position; i++){
		next = next->nextNode;
		pre = pre->nextNode;
	}
	insertNode->previousNode = pre;
	insertNode->nextNode = next;
	insertNode->data = value;
	pre->nextNode = insertNode;
	next->previousNode = insertNode;
}

void deleteNode (int position,node* originNode)
{
	if (position > 0){
		node* ptr = originNode;
		for (int i=0; i<position-1; i++){
			ptr = ptr -> nextNode;
		}
		node* temp;
		node* delete;
		delete = ptr->nextNode;
		temp = ptr->nextNode->nextNode;
		ptr->nextNode = temp;
		temp->previousNode = ptr;
		deleteAllSubNodes(delete);
		free(delete);
	}
}

void deleteAllSubNodes (node* targetNode)
{
	subNode* current = targetNode->subNodeHead;
	subNode* next;
	while(current != NULL){
		next = current->nextSubNode;
		free(current);
		current = next;
	}
	targetNode->subNodeHead = NULL;
	//targetNode->subNodeHead = (subNode*)malloc(sizeof(subNode));
}

void deleteList(node* originNode)
{
	node* current = originNode->nextNode;
	node* next;

	while (current != originNode){
		deleteAllSubNodes(current);
		next = current->nextNode;
		free(current);
		current = next;
		originNode->nextNode = current;
	}
	originNode->previousNode = originNode;
	deleteAllSubNodes(originNode);
}

//Print list has been implemented for you.
// You may study the functions below to help you implement your own functions.
void printList(node* originNode)
{
	int count = 0;
	printf("Printing clockwise:\n");
	node* iterator = originNode->nextNode;
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);
	printf("\n   |\n   v\n");
	while(iterator != originNode)
	{
		count++;
		printf("[Pos %i:%i]",count,iterator->data);
		// printing subNodes
		printSubNodes(iterator);
		printf("\n   |\n   v\n");
		iterator = iterator->nextNode;
	}
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);

	printf("\nPrinting counter-clockwise:\n");
	iterator = originNode->previousNode;
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);
	printf("\n   |\n   v\n");
	while(iterator!= originNode)
	{
		printf("[Pos %i:%i]",count,iterator->data);
		// printing subNodes
		printSubNodes(iterator);
		printf("\n   |\n   v\n");
		iterator = iterator->previousNode;
		count--;
	}
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);
	printf("\n");

	return;
}

void printSubNodes(node* mainNode)
{
	int count = 0;
	subNode* iterator = mainNode->subNodeHead;
	while(iterator != NULL)
	{
		printf("->[subNode pos %i:%i]",count,iterator->data);
		iterator = iterator->nextSubNode;
		count++;
	}
}

#include <stdio.h>
#include <stdlib.h>
// stdlib includes malloc() and free()

// define instructions
#define PREVIOUS -1
#define NEXT 1
#define DELETE 0


// Node declaration ( Do not edit )
typedef struct NODE
{
	int data;
	struct NODE* previousNode;
	struct NODE* nextNode;
} node;

// Function prototypes
void insertNodeNext(int,int,node*);//TODO
void insertNodePrevious(int,int,node*);// TODO
void deleteNode(int,node*); // TODO
void deleteList(node*); // TODO

void printList(node*); // Implemented for you

// Start of main
int main()
{
	int position;
	int instruction;
	int value;

	// Declaration of the origin Node ( do not edit )
	node* originNode = (node*)malloc(sizeof(node));
	originNode->previousNode = originNode;
	originNode->nextNode = originNode;
	originNode->data = 0;

	// input and processing has been done for you.

	while(scanf("%i%i",&instruction,&position) == 2)
		//scanf() returns an int that corresponds to the number of values scanned.
	{
		if(instruction == DELETE)
		{
			deleteNode(position,originNode);
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
		free(delete);
	}

}

void deleteList(node* originNode)
{
	/*
	node* ptr = originNode;
	while (ptr->nextNode != originNode){
		node* temp;
		node* delete;
		delete = ptr->nextNode;
		temp = ptr->nextNode->nextNode;
		ptr->nextNode = temp;
		temp->previousNode = ptr;
		free(delete);
	}
	*/
	node* current = originNode->nextNode;
	node* next;

	while (current != originNode){
		next = current->nextNode;
		free(current);
		current = next;
		originNode->nextNode = current;
	}
	originNode->previousNode = originNode;


}

// printList has been implemented for you.
// You may study the functions below to help you implement your own functions.
void printList(node* originNode)
{
	int count = 0;

	printf("Printing clockwise:\n");
	node* iterator = originNode->nextNode;
	printf("[Pos %i:%i]",0,originNode->data);
	printf("\n   |\n   v\n");
	while(iterator != originNode)
	{
		count++;
		printf("[Pos %i:%i]",count,iterator->data);
		printf("\n   |\n   v\n");
		iterator = iterator->nextNode;
	}
	printf("[Pos %i:%i]",0,originNode->data);

	printf("\nPrinting counter-clockwise:\n");
	iterator = originNode->previousNode;
	printf("[Pos %i:%i]",0,originNode->data);
	printf("\n   |\n   v\n");
	while(iterator!= originNode)
	{
		printf("[Pos %i:%i]",count,iterator->data);
		printf("\n   |\n   v\n");
		iterator = iterator->previousNode;
		count--;
	}
	printf("[Pos %i:%i]\n",0,originNode->data);

	return;
}

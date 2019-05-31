#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdexcpt.h>

#define NO_VALUE 9999;

typedef struct Node* Node_pointer;

typedef struct Node {
	size_t start;
	Node_pointer next;
	Node_pointer before;
} Node;


typedef struct BuddyList* BuddyListPointer;

typedef struct BuddyList {
	size_t scale;
	BuddyListPointer upper;
	BuddyListPointer lower;
	Node_pointer HEAD;
} BuddyList;



Node_pointer init_list();
void add_node(size_t start, Node_pointer HEAD, size_t scale);
void remove_node(Node_pointer HEAD, size_t start);
size_t pop_node(Node_pointer HEAD);
Node_pointer getScaleHEAD(BuddyListPointer buddypointer, size_t scale);
BuddyListPointer init_buddy_list();

BuddyListPointer buddyHEAD;

Node_pointer init_list() //return HEAD NODE
{
	Node_pointer HEAD = malloc(sizeof(Node));
	HEAD->start = NO_VALUE; //initialize HEAD to 9999(NOVALUE)
	HEAD->next = NULL;
	HEAD->before = NULL;
	return HEAD;
}

void add_node(size_t start, Node_pointer HEAD, size_t scale)
{
	Node_pointer Next = HEAD;
	Node_pointer node = (Node_pointer)malloc(sizeof(Node));

	node->start = start;

	if (HEAD->next == NULL) {
		HEAD->next = node;
		node->before = HEAD;
		node->next = NULL;
		return;
	}

	Next = Next->next;

	while (Next->start< start) {
		if (Next->next != NULL) {
			Next = Next->next;
		}
		else {
			break;
		}
	}
	
	if (Next->next == NULL && Next->start < start) {
		node->before = Next;
		node->next = NULL;
		Next->next = node;

		// merge buddies
		Node_pointer b = node->before;

		if (b != NULL && b != HEAD && b->start + scale == node->start && (b->start / scale) % 2 == 0) {
			//merge
			add_node(b->start, getScaleHEAD(buddyHEAD, scale * 2), scale * 2);
			remove_node(HEAD, b->start);
			remove_node(HEAD, node->start);

		}
		return;
	}
	else {
		node->next = Next;
		Next->before->next = node;
		node->before = Next->before;

		// merge buddies
		Node_pointer b = node->before;
		Node_pointer n = node->next;

		if (b != NULL && b != HEAD && b->start + scale == node->start && (b->start / scale) % 2 == 0) {
			//merge
			add_node(b->start, getScaleHEAD(buddyHEAD, scale * 2), scale * 2);
			remove_node(HEAD, b->start);
			remove_node(HEAD, node->start);

		}
		if (n != NULL && node->start + scale == n->start && ((n->start + scale) / scale) % 2 == 1) {
			//merge
			add_node(node->start, getScaleHEAD(buddyHEAD, scale * 2), scale * 2);
			remove_node(HEAD, node->start);
			remove_node(HEAD, n->start);
		}
	}

	return;
}

void remove_node(Node_pointer HEAD, size_t start) {
	Node_pointer Next = HEAD->next;
	if (Next == NULL) {
		printf("[ERROR] There is no node(EMPTY LIST ONLY HEADER).\n");
		return;
	}
	while (Next->start != start) {
		Next = Next->next;
		if (Next == NULL) {
			printf("[ERROR] There is no node that has start=%d\n", start);
			return;
		}
	}
	if (Next->next == NULL) { // if there is a only one node.
		HEAD->next = NULL;
		free(Next);
		return;
	}
	Next->next->before = Next->before;
	Next->before->next = Next->next;
	free(Next);
	
	return;
}

size_t pop_node(Node_pointer HEAD)
{
	size_t start;
	Node_pointer Next = HEAD->next;
	if (Next == NULL) {
		printf("[ERROR] There is no node(EMPTY LIST ONLY HEADER).\n");
		return NO_VALUE;
	}
	if (Next->next == NULL) { // if there is a only one node.
		HEAD->next = NULL;
		free(Next);
		return NO_VALUE;
	}
	start = Next->start;
	HEAD->next = Next->next;
	Next->next->before = HEAD;
	free(Next);

	return start;
}


Node_pointer getScaleHEAD(BuddyListPointer buddypointer, size_t scale)
{
	BuddyListPointer Next = buddypointer;
	while (Next->scale != scale)
		Next = Next->upper;
	return Next->HEAD;
}

BuddyListPointer init_buddy_list() {
	BuddyListPointer BuddyHEAD = malloc(sizeof(BuddyList));
	BuddyListPointer current = BuddyHEAD;
	BuddyListPointer blp = malloc(sizeof(BuddyList));
	int ScaleArray[9] = { 1,2,4,8,16,32,64,128,256 };
	int i;

	current->scale = NO_VALUE;
	current->lower = NULL;
	current->upper = blp;
	blp->lower = current;
	blp->HEAD = init_list();

	current = current->upper;

	for (i = 0; i < 9; i++) {
		BuddyListPointer blp2 = malloc(sizeof(BuddyList));
		current->scale = ScaleArray[i];
		current->HEAD = init_list();
		if (i == 8) {
			current->upper = NULL;
			break;
		}
		current->upper = blp2;
		blp2->lower = current;

		current = current->upper;
	}

	return BuddyHEAD;
}

void print_status() {
	BuddyListPointer NextChannel = buddyHEAD->upper;
	while (NextChannel->upper != NULL) {
		printf("SCALE : %d : ", NextChannel->scale);
		Node_pointer NextNode = NextChannel->HEAD->next;
		while (NextNode != NULL) {
			printf("%d ", NextNode->start);
			NextNode = NextNode->next;
		}
		printf("\n");
		NextChannel = NextChannel->upper;
	}
}

int main()
{
	buddyHEAD = init_buddy_list();
	add_node(0, getScaleHEAD(buddyHEAD, 32),32);
	add_node(64, getScaleHEAD(buddyHEAD, 32), 32);
	add_node(32, getScaleHEAD(buddyHEAD, 32), 32);
	print_status();
}
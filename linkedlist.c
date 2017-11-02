#include <stdlib.h>
#include "linkedlist.h"

/* Insert at end of doubly linked list */
void insert_tail(NodePtr *head, NodePtr *tail, int data) {

	NodePtr tmp = malloc(sizeof(Node));
	tmp->data = data;
	tmp->next = NULL;

	/* Inserting into empty list */
	if(*head==NULL) {
		tmp->prev = NULL;
		*head = *tail = tmp;
		return;
	}

	tmp->prev = *tail;
	(*tail)->next = tmp;
	*tail = tmp;
}

/*Insert at the beginning of doubly linked list */
void insert_head(NodePtr *head, NodePtr *tail, int data) {
	NodePtr tmp = malloc(sizeof(Node));
	tmp->data = data;
	tmp->prev = NULL;

	if(*head==NULL) {
		tmp->next = NULL;
		*head = *tail = tmp;
		return;
	}
	tmp->next = *head;
	(*head)->prev = tmp;
	*head = tmp;
}

/* Deletes Node pointed to by "node" */
void delete_list(NodePtr *head, NodePtr *tail, NodePtr node) {
	if(*head == *tail && *head==node) {
		*head = *tail = NULL;
	} else if(node == *head) {
		*head = node->next;
		(*head)->prev = NULL;
	} else if(node == *tail) {
		*tail = node->prev;
		(*tail)->next = NULL;
	} else {
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}
	free(node);
}

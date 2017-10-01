#include <stdlib.h>
#include "linkedlist.h"

/* Insert at end of doubly linked list */
void insert_list(NodePtr *head, NodePtr *tail, int data) {

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

void delete_list(NodePtr node) {
	if(node->prev!=NULL)
		node->prev->next = node->next;
	if(node->next!=NULL)
		node->next->prev = node->prev;
	free(node);
}

#include <string.h>
#include <stdio.h>
#include "biginteger.h"

void insert_lsb(BigInteger *, int);
void insert_msb(BigInteger *, int);
void complement(BigInteger *);
void remove_leading_zero(BigInteger *);
int stoi(char *str);

/* Converting a string to integer */
int stoi(char *str) {

	int n;
	sscanf(str,"%d",&n);
	return n;
}

void insert_msb(BigInteger *x, int data) {
	++(x->length);
	insert_tail(&(x->lsb), &(x->msb), data);
}

void insert_lsb(BigInteger *x, int data) {
	++(x->length);
	insert_head(&(x->lsb), &(x->msb), data);
}

void remove_leading_zero(BigInteger *x) {

	while(x->length > 1 && x->msb->data==0) {
		delete_list(&(x->lsb), &(x->msb), x->msb);
		--(x->length);
	}
	/* In case our number is just a 0, set sign to 0*/
	if(x->length==1 && x->msb->data==0)
		x->sign = 0;
}

/* Sign extends until there are the specified number of
 * groups of 4 digits in the linked list */
void sign_extend(BigInteger *x, int groups) {
	int diff = groups - x->length;
	while(diff > 0) {
		insert_msb(x,0);
		--diff;
	}
}

/* Find r's complement of x */
void complement(BigInteger *x) {
	NodePtr i;
	for(i=x->lsb;i!=NULL;i=i->next) {
		i->data = 10000 - i->data - 1;
	}

	int carry = 1; // Add 1 to the (r-1)'s complement
	for(i=x->lsb;i!=NULL && carry!=0;i=i->next) {
		i->data = i->data + carry;
		if(i->data >= 10000) {
			carry = i->data / 10000;
			i->data = i->data % 10000;
		} else {
			carry = 0;
		}
	}
}

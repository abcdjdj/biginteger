#include <string.h>
#include <stdio.h>
#include "biginteger.h"

void insert_lsb(BigInteger *, int);
void insert_msb(BigInteger *, int);
void complement(BigInteger *);
void remove_leading_zero(BigInteger *);
int stoi(char *str);

/*
 * Function:  stoi
 * --------------------
 *  Converts the given string to an int
 *  str : string that contains only digits
 *
 *  returns: a native int equivalent of str
 */
int stoi(char *str) {

	int n;
	sscanf(str,"%d",&n);
	return n;
}

/*
 * Function:  insert_msb
 * --------------------
 *  Inserts the given int at the most significant position (tail of list)
 *  x : BigInteger upon which insertion has to take place
 *  data : the int to be inserted
 *
 *  returns: nothing (void)
 */
void insert_msb(BigInteger *x, int data) {
	++(x->length);
	insert_tail(&(x->lsb), &(x->msb), data);
}

/*
 * Function:  insert_lsb
 * --------------------
 *  Inserts the given int at the least significant position (head of list)
 *  x : BigInteger upon which insertion has to take place
 *  data : the int to be inserted
 *
 *  returns: nothing (void)
 */
void insert_lsb(BigInteger *x, int data) {
	++(x->length);
	insert_head(&(x->lsb), &(x->msb), data);
}

/*
 * Function:  remove_leading_zero
 * --------------------
 *  Removes all leading zeros (at msb end) from the list
 *  x : BigInteger upon which deletion has to take place
 *
 *  returns: nothing (void)
 */
void remove_leading_zero(BigInteger *x) {

	while(x->length > 1 && x->msb->data==0) {
		delete_list(&(x->lsb), &(x->msb), x->msb);
		--(x->length);
	}
	/* In case our number is just a 0, set sign to 0*/
	if(x->length==1 && x->msb->data==0)
		x->sign = 0;
}

/*
 * Function:  sign_extend
 * --------------------
 *  Adds leading 0s until there are the specified number of
 *  digits/nodes in the linked list
 *  x : BigInteger upon which insertion has to take place
 *  groups : the final length of the linked list after inserting 0s
 *
 *  returns: nothing (void)
 */
void sign_extend(BigInteger *x, int groups) {
	int diff = groups - x->length;
	while(diff > 0) {
		insert_msb(x,0);
		--diff;
	}
}

/*
 * Function:  complement
 * --------------------
 *  Computes r's complement of the given BigInteger and overwrites it
 *  Here r = Radix = 10000
 *  x : BigInteger to be complemented
 *
 *  returns: nothing (void)
 */
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utilities.h"
#include "biginteger.h"

/* Construct BigInteger from a char array */
BigInteger *init(char *str) {

	BigInteger *bi = malloc(sizeof(BigInteger));
	bi->msb = bi->lsb = NULL;
	bi->length = 0;

	/* Initialize sign bit */
	if(*str=='-'){
		bi->sign = 1;
		++str;
	} else {
		bi->sign = 0;
	}

	int i, l = strlen(str);

	char tmp[5];
	tmp[4] = '\0';

	int j = 3, ans;
	for(i = l-1; i>=0; --i) {
		if(j==0) {
			tmp[j] = str[i];
			ans = stoi(tmp);
			insert(bi, ans);
			j=3;
		} else {
			tmp[j] = str[i];
			--j;
		}
	}
	while(j>=0) {
		tmp[j--] = '0';
	}
	ans = stoi(tmp);
	if(ans!=0) {
		insert(bi, ans);
	}
	return bi;
}

/* Deep clones a BigInteger object */
BigInteger *clone(BigInteger *x) {
	BigInteger *ans = malloc(sizeof(BigInteger));
	ans->sign = x->sign;
	ans->msb = ans->lsb = NULL;
	ans->length = 0;

	NodePtr i;
	for(i=x->lsb;i!=NULL;i=i->next)
		insert(ans, i->data);
	return ans;
}

void insert(BigInteger *x, int data) {
	++(x->length);
	insert_list(&(x->lsb), &(x->msb), data);
}

/* Sign extends until there are the specified number of
 * groups of 4 digits in the linked list */
void sign_extend(BigInteger *x, int groups) {
	int diff = groups - x->length;
	while(diff > 0) {
		insert(x,0);
		--diff;
	}
}

/* Find r's complement of x */
void complement(BigInteger *x) {
	NodePtr i;
	for(i=x->lsb;i!=NULL;i=i->next) {
		i->data = 10000 - i->data - 1;
	}
	add_int(x,1);
}

/* Computes x-y using the r's complement method */
BigInteger *subtract_magnitude(BigInteger *x, BigInteger *y) {

	BigInteger *y_compl = clone(y);
	sign_extend(y_compl, x->length);
	complement(y_compl);
	BigInteger *difference = add_magnitude(x,y_compl,1);
	free(y_compl);
	return difference;
}

/* Compares the absolute value of x & y */
int compare_abs(BigInteger *x, BigInteger *y) {
	if(x->length > y->length) {
		return 1;
	} else if(x->length < y->length) {
		return -1;
	}
	NodePtr i,j;
	for(i=x->msb, j=y->msb; i!=NULL; i=i->prev, j=j->prev) {
		if(i->data > j->data)
			return 1;
		else if(i->data < j->data)
			return -1;
	}
	return 0;
}

/* Adds a normal int to the BigInteger */
void add_int(BigInteger *x, int data) {
	int carry = data;
	NodePtr i;
	for(i=x->lsb;i!=NULL && carry!=0;i=i->next) {
		i->data = i->data + carry;
		if(i->data >= 10000) {
			carry = i->data / 10000;
			i->data = i->data % 10000;
		} else {
			carry = 0;
		}
	}
	if(carry!=0) {
		insert(x, carry);
	}
}

BigInteger *add(BigInteger *x, BigInteger *y) {
	BigInteger *ans;
	if(x->sign == y->sign) {
		ans = add_magnitude(x,y,0);
		ans->sign = x->sign;
		return ans;
	}
	BigInteger *larger, *smaller;
	if(compare_abs(x,y)==1) {
		larger = x;
		smaller = y;
	} else {
		larger = y;
		smaller = x;
	}
	ans = subtract_magnitude(larger, smaller);
	ans->sign = larger->sign;
	return ans;
}

/* For internal use only
 * WARNING :- Strictly adds only magnitudes */
BigInteger *add_magnitude(BigInteger *x, BigInteger *y, int ignoreCarry) {
	BigInteger *c = malloc(sizeof(BigInteger));
	c->msb = c->lsb = NULL;
	c->sign = 0;
	c->length = 0;
	NodePtr a,b;
	a = x->lsb;
	b = y->lsb;
	int carry = 0, ans;
	while(a || b) {
		if(a==NULL) {
			ans = b->data + carry;
			b = b->next;
		} else if(b==NULL) {
			ans = a->data + carry;
			a = a->next;
		} else {
			ans = a->data + b->data + carry;
			a = a->next;
			b = b->next;
		}
		if(ans>=10000){
			carry = ans / 10000;
			ans = ans % 10000;
		} else {
			carry = 0;
		}
		insert(c, ans);
	}

	if(carry > 0 && !ignoreCarry)
		insert(c, carry);
	return c;
}

void delete(BigInteger *x) {
	NodePtr i,tmp;
	for(i=x->lsb;i!=NULL;i=tmp) {
		tmp = i->next;
		free(i);
	}
	free(x);
}

void display(BigInteger *bi) {

	NodePtr i;
	if(bi->sign)
		printf("-");
	printf("%d",bi->msb->data);
	for(i=bi->msb->prev; i!=NULL; i=i->prev)
		printf("%04d", i->data);
	printf("\n");
}

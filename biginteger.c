#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utilities.h"
#include "biginteger.h"

/* Static function declarations */
static void insert(BigInteger *, int);
static int compare_abs(BigInteger *, BigInteger *);
static void complement(BigInteger *);
static BigInteger *add_magnitude(BigInteger *, BigInteger *, int);
static BigInteger *subtract_magnitude(BigInteger *, BigInteger *);
static void multiply_int(BigInteger *, int);
static void add_overwrite(BigInteger *, BigInteger *);
/* End of static function declarations */

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
	if(ans!=0  || bi->length == 0) {
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

static void insert(BigInteger *x, int data) {
	++(x->length);
	insert_list(&(x->lsb), &(x->msb), data);
}

/* Sign extends until there are the specified number of
 * groups of 4 digits in the linked list */
static void sign_extend(BigInteger *x, int groups) {
	int diff = groups - x->length;
	while(diff > 0) {
		insert(x,0);
		--diff;
	}
}

/* Find r's complement of x */
static void complement(BigInteger *x) {
	NodePtr i;
	for(i=x->lsb;i!=NULL;i=i->next) {
		i->data = 10000 - i->data - 1;
	}
	add_int(x,1);
}

BigInteger *subtract(BigInteger *x, BigInteger *y) {
	BigInteger *ans;
	if(x->sign != y->sign) {
		ans = add_magnitude(x, y, 0);
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
	ans->sign = (larger==x)?(x->sign):(!x->sign);

	return ans;
}

/* Computes x-y using the r's complement method */
static BigInteger *subtract_magnitude(BigInteger *x, BigInteger *y) {

	BigInteger *y_compl = clone(y);
	sign_extend(y_compl, x->length);
	complement(y_compl);
	BigInteger *difference = add_magnitude(x,y_compl,1);
	delete(y_compl);
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

/* Warning :- q has to be at most 4 digits long to
 * or else int will overflow!! */
static void multiply_int(BigInteger *x, int q) {
	NodePtr i;
	int carry = 0;
	for(i = x->lsb; i!=NULL; i=i->next) {
		i->data = i->data * q + carry;
		carry = i->data / 10000;
		i->data = i->data % 10000;
	}
	if(carry!=0)
		insert(x, carry);
}

BigInteger *multiply(BigInteger *x, BigInteger *y) {
	BigInteger *multiplier, *multiplicand;
	if(x->length > y->length) {
		multiplier = y;
		multiplicand = x;
	} else {
		multiplier = x;
		multiplicand = y;
	}
	BigInteger *ans = init("0");
	NodePtr i, original_lsb = ans->lsb;
	for(i=multiplier->lsb; i!=NULL; i=i->next) {
		BigInteger *partial_prod = clone(multiplicand);
		multiply_int(partial_prod, i->data);
		add_overwrite(ans, partial_prod);

		ans->lsb = ans->lsb->next;
		delete(partial_prod);
	}
	ans->lsb = original_lsb;
	ans->sign = x->sign ^ y->sign;
	return ans;
}

static void add_overwrite(BigInteger *x, BigInteger *y) {
	NodePtr a,b;
	a = x->lsb;
	b = y->lsb;
	int carry = 0;
	while(a || b) {
		if(b==NULL) {
			a->data = a->data + carry;
			carry = a->data / 10000;
			a->data = a->data % 10000;
			a=a->next;
		} else if(a==NULL) {
			int ans = b->data + carry;
			carry = ans/10000;
			ans=ans%10000;
			b=b->next;
			insert(x, ans);
		} else {
			a->data = a->data + b->data + carry;
			carry = a->data / 10000;
			a->data = a->data % 10000;
			a=a->next;
			b=b->next;
		}
	}
	if(carry > 0)
		insert(x, carry);
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
static BigInteger *add_magnitude(BigInteger *x, BigInteger *y, int ignoreCarry) {
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

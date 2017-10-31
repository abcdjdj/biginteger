#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utilities.h"

/* Static function declarations */
static BigInteger *add_magnitude(BigInteger *, BigInteger *, int);
static BigInteger *subtract_magnitude(BigInteger *, BigInteger *);
static BigInteger *divide_magnitude(BigInteger *, BigInteger *);
static void multiply_int(BigInteger *, int);
static void add_overwrite(BigInteger *, BigInteger *, int);
static void subtract_overwrite(BigInteger *, BigInteger *);
static int compare_abs(BigInteger *, BigInteger *);
/* End of static function declarations */


/* +++++++++++++++++++++++++++ Constructors +++++++++++++++++++++++++++ */

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
			insert_msb(bi, ans);
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
		insert_msb(bi, ans);
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
		insert_msb(ans, i->data);
	return ans;
}
/* ++++++++++++++++++++++++ End of Constructors +++++++++++++++++++++++ */
/* ++++++++++++++++++++++++ Internal static functions +++++++++++++++++ */

/* Computes x-y using the r's complement method */
static BigInteger *subtract_magnitude(BigInteger *x, BigInteger *y) {

	BigInteger *y_compl = clone(y);
	sign_extend(y_compl, x->length);
	complement(y_compl);
	BigInteger *difference = add_magnitude(x,y_compl,1);
	delete(y_compl);
	return difference;
}

/* Subtracts y from x and overwrites contents of x */
static void subtract_overwrite(BigInteger *x, BigInteger *y) {
	sign_extend(y, x->length);
	complement(y);
	add_overwrite(x, y, 1);
	complement(y);
	remove_leading_zero(x);
	remove_leading_zero(y);
}


/* Compares the absolute value of x & y */
static int compare_abs(BigInteger *x, BigInteger *y) {
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

/* Divides y by x and returns the quotient */
static BigInteger *divide_magnitude(BigInteger *n, BigInteger *d) {

	if(compare_abs(n,d) < 0)
		return init("0");
	BigInteger *quotient = malloc(sizeof(BigInteger));
	quotient->sign = 0;
	quotient->msb = quotient->lsb = NULL;
	quotient->length = 0;

	/* Divisor lookup table for multiplication */
	BigInteger *table[9999];
	int i;
	for(i=0; i <9999; ++i) {
		table[i] = clone(d);
		multiply_int(table[i], i+1);
		table[i]->sign = 0;
	}

	BigInteger tmp, *wrapper = &tmp; // tmp is on the stack, not heap
	wrapper->sign = 0;
	wrapper->length = 1;
	wrapper->msb = wrapper->lsb  = n->msb;
	NodePtr nextDividend = wrapper->lsb->prev;
	wrapper->lsb->prev = NULL;
	do {
		int count = 0;
		/* Add next digit(s) of dividend if required */
		while(compare_abs(wrapper, d) < 0 && nextDividend) {
			++count;

			wrapper->lsb->prev = nextDividend;
			wrapper->lsb = nextDividend;
			nextDividend = wrapper->lsb->prev;
			wrapper->lsb->prev = NULL;
			++(wrapper->length);

			if(count>=2) {
				insert_lsb(quotient, 0);
			}
			remove_leading_zero(wrapper);
		}

		if(compare_abs(wrapper, d) < 0) {
			insert_lsb(quotient, 0);
			break; // Means that nextDividend is null i.e end of dividend
		}

		int upper = 9999-1, lower = 0, mid;
		while(upper >= lower) {
			mid = (upper + lower)/2;
			if(compare_abs(wrapper, table[mid]) >= 0) {
				i = mid;
				lower = mid + 1;
			} else {
				upper = mid - 1;
			}
		}

		subtract_overwrite(wrapper, table[i]);
		insert_lsb(quotient, i+1);

	} while(nextDividend!=NULL);

	/* Free up the divisor table */
	for(i=0; i <9999; ++i)
		delete(table[i]);
	return quotient;
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
		insert_msb(x, carry);
}


static void add_overwrite(BigInteger *x, BigInteger *y, int ignoreCarry) {
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
			insert_msb(x, ans);
		} else {
			a->data = a->data + b->data + carry;
			carry = a->data / 10000;
			a->data = a->data % 10000;
			a=a->next;
			b=b->next;
		}
	}
	if(carry > 0 && !ignoreCarry)
		insert_msb(x, carry);
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
		carry = ans / 10000;
		ans = ans % 10000;
		insert_msb(c, ans);
	}

	if(carry > 0 && !ignoreCarry)
		insert_msb(c, carry);
	return c;
}
/* +++++++++++++++++++ End of static function ++++++++++++++++++++++ */
/* +++++++++++++++++++ Exposed functions +++++++++++++++++++++++++++ */

BigInteger *subtract(BigInteger *x, BigInteger *y) {
	BigInteger *ans;
	if(x->sign != y->sign) {
		ans = add_magnitude(x, y, 0);
		ans->sign = x->sign;
		remove_leading_zero(ans);
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
	remove_leading_zero(ans);

	return ans;
}


BigInteger *divide(BigInteger *n, BigInteger *d) {
	if(d->length==1 && d->msb->data==0)
		return NULL;
	BigInteger *quotient = divide_magnitude(n, d);
	quotient->sign = n->sign ^ d->sign;
	remove_leading_zero(quotient);
	return quotient;
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
		add_overwrite(ans, partial_prod, 0);

		ans->lsb = ans->lsb->next;
		delete(partial_prod);
	}
	ans->lsb = original_lsb;
	ans->sign = x->sign ^ y->sign;
	remove_leading_zero(ans);
	return ans;
}


BigInteger *add(BigInteger *x, BigInteger *y) {
	BigInteger *ans;
	if(x->sign == y->sign) {
		ans = add_magnitude(x,y,0);
		ans->sign = x->sign;
		remove_leading_zero(ans);
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
	remove_leading_zero(ans);
	return ans;
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

/* ++++++++++++++++++ End of exposed functions ++++++++++++++++++++ */

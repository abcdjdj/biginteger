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

}

/* Find r's complement of x */
BigInteger *complement(BigInteger *x) {
	BigInteger *ans = x; //= clone(x);
	NodePtr i;

	for(i=ans->lsb;i!=NULL;i=i->next) {
		i->data = 10000 - i->data - 1;
	}
	ans->sign = 0;
	add_int(ans,1);
	return ans;
}

BigInteger *subtract_magnitude(BigInteger *x, BigInteger *y) {
	complement(y);
	printf("INSDE SUB_MAG = ");
	display(y);
	return add_magnitude(x,y,1);
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
	}
}

/* For internal use only
 * WARNING :- Strictly adds only magnitudes */
BigInteger *add_magnitude(BigInteger *x, BigInteger *y, int ignoreCarry) {
	BigInteger *c = malloc(sizeof(BigInteger));
	c->length = 0;
	NodePtr a,b;
	a = x->lsb;
	b = y->lsb;
	int carry = 0;
	while(a || b) {
		if(a==NULL) {
			insert(c, b->data + carry);
			b = b->next;
			carry = 0;
		} else if(b==NULL) {
			insert(c, a->data + carry);
			a = a->next;
			carry = 0;
		} else {
			int ans = a->data + b->data + carry;
			if(ans >= 10000) {
				carry = ans / 10000;
				ans = ans % 10000;
			} else {
				carry = 0;
			}
			insert(c, ans);
			a = a->next;
			b = b->next;
		}
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
	for(i=bi->msb; i!=NULL; i=i->prev)
		printf("%d", i->data);
	printf("\n");
}

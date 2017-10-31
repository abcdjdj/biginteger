#include "linkedlist.h"

#define RADIX 10000
/* Using Radix as 10000 */

typedef struct {
	NodePtr msb;
	NodePtr lsb;
	int sign;
	int length;
} BigInteger;

BigInteger *init(char *);

void display(BigInteger *);

BigInteger *add(BigInteger *, BigInteger *);

BigInteger *subtract(BigInteger *, BigInteger *);

BigInteger *multiply(BigInteger *, BigInteger *);

BigInteger *divide(BigInteger *n, BigInteger *d);

int compare(BigInteger *, BigInteger *);

BigInteger *clone(BigInteger *);

void delete(BigInteger *);

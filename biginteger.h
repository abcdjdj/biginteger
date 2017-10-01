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

BigInteger *clone(BigInteger *);

void add_int(BigInteger *, int);

void delete(BigInteger *);

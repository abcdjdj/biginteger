#include "linkedlist.h"

#define RADIX 10000
/* Using Radix as 10000 */

typedef struct {
	NodePtr msb;
	NodePtr lsb;
	int sign;
} BigInteger;

BigInteger *init(char *);

void display(BigInteger *);

BigInteger *add(BigInteger *, BigInteger *);

BigInteger *add_magnitude(BigInteger *, BigInteger *, int);

BigInteger *subtract_magnitude(BigInteger *, BigInteger *);

BigInteger *clone(BigInteger *);

BigInteger *complement(BigInteger *);

void add_int(BigInteger *, int);

void delete(BigInteger *);

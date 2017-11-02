#include <stdio.h>
#include "biginteger.h"

int main() {

	char s1[1024], s2[1024];
	printf("Enter two numbers\n");

	while(scanf("%s %s",s1, s2)!=EOF) {
		BigInteger *a = init(s1);
		BigInteger *b = init(s2);
		BigInteger *c = subtract(a,b);
		printf("Difference = ");
		display(c);
		delete(c);
		delete(a);
		delete(b);
		printf("Enter two numbers\n");

	}
}

#include <string.h>
#include <stdio.h>

void strrev(char *str);

int stoi(char *str);

/* strrev() is not available in
 * Linux's glibc */
void strrev(char *str) {

	char tmp;
	int i,l = strlen(str);

	for(i=0;i<l/2;++i) {
		tmp = str[i];
		str[i] = str[l-i-1];
		str[l-i-1] = tmp;
	}
}

/* Converting a string to integer */
int stoi(char *str) {

	int n;
	sscanf(str,"%d",&n);
	return n;
}

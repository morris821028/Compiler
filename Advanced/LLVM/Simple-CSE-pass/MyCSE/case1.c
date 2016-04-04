#include <stdio.h>

int main(void) {
	int a = 30;
	int b = 40;
	int c = a + b + 10;
	int d = (a + b) * c;
	printf("%d\n", c + d);
	if (a == 30) 
		return 1;
	a = 40;
	b = 30;
	return 0;
}

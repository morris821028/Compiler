/*
Test for common subexpression elimination.
*/
#include <stdio.h>
int main(void){
	int i, j, x, y, z, a, b, c, d, e;
	unsigned int checksum = 0;
	a = 10; b = 10; c = 10; d = 1; e = 1;
	for (i = 0; i < 5000; i += 1){
		for (j = 0; j < 50000; j += 1){
			x = a * b / c;
			y = a * b / c + d;
			z = a * b / c + d - e; 
			checksum += x + y + z;
		}
		a += 1; b += 1; c += 1; d += 1; e += 1;
	}
	printf("%u\n", checksum);
	return 0;
}


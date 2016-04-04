#include <stdio.h>

#define n 1024
double a[n*n], b[n*n];
int main() {
	int i, j, k;
	for (i = 1; i < n-1; i++) {
		for (j = 1; j < n-1; j++) {
			b[i*n+j] = (
				5 * a[i*n+j] + a[(i-1)*n+j] + a[i*n+(j-1)]
			 	+ a[(i+1)*n+j] + a[i*n+(j+1)]
			)/9;
		}
	}
	return 0;
}

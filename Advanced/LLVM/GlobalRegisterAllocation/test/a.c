#include <stdio.h>

int f(int a, int b) {
	return a + b;
}
int main() {
	int a = 5, b = 10;
	int c = f(a, b);
	printf("%d\n", c);
	return 0;
}

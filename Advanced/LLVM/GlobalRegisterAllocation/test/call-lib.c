#include <stdio.h>

int f(int a, int b, int c[]) {
	int sum = 0;
	for (int i = 0; i < a; i++)
		c[i] = i * b;
	for (int i = 0; i < a; i++)
		sum += c[i];
	return sum;
}
int main() {
	int a = 5, b = 10, c[32767] = {};
	int sum = f(a, b, c);
	printf("%d\n", sum);
	return 0;
}

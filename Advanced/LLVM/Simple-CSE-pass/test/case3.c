#include <stdio.h>

int f(int a, int b) {
	int x = (a * 40) + 2;
	int y = a + b;
	int z = (a * 40 + 161);
	return (a > 0 ? x : z);
}
int main() {
	return f(514, 1024);
}

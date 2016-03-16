#include <stdio.h>

int foo(int *arg) {
	const int a = 5;
	int b;
	if (a > 10 && arg == NULL)
		b = 10;
	else
		b = 20;
	arg = NULL;
	return b;
}
int main() {
	return foo(NULL);
}

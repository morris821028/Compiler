#include <stdio.h>
#include <stdbool.h>
int f(int *arg) {
	int ret = 0;
	int a = 0, b = 0;
	if (false)
		ret = 2;
	if (*arg == 1)
		ret = 1;
	return 0;
}

int main(void) {
	int v = 1;
	return f(&v);
}

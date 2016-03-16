#include <stdio.h>

int main(void) {
	int a = 1, b = 1000000;
	if (a < 0)
		for (int i = 0; i < b; i++);
	else
		for (int j = 0; j < b; j++);
	return 1;
}

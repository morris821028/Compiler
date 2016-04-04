#include <stdio.h>

int f(int a) {
	int x = (a * 4) + 2;
	int y = (a * 4) + 1;
	return (a ? x : y);
}
int main() {
	return f(1024);
}

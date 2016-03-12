#include <stdio.h>

int main(void) {
	int a = 1;
	if (a)
		goto Hell;
	goto AdvHell;
	{
		int nohell = 0;
	}
	return 0;
Hell:
	switch(a) {
		case 1: {
			int b = 1;
			return b;
		}
	}
	return 2;
AdvHell:
	return 3;
}

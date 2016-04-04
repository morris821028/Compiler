/*
Test for common subexpression elimination.
*/

void print_int(int i) {}
void print_newline(void) {}

int main(void){
	int i, j, x, y, z, a, b, c, d, e;
	a = 10; b = 10; c = 10; d = 1; e = 1;
	for (i = 0; i < 5000; i += 1){
		for (j = 0; j < 50000; j += 1){
			x = a * b / c;
			y = a * b / c + d;
			z = a * b / c + d - e; 
		}
		a += 1; b += 1; c += 1; d += 1; e += 1;
	}
	print_int(x + y + z); print_newline();
	return 0;
}


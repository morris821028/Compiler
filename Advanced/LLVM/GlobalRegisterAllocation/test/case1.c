#include <stdio.h>

long long mul(long long a, long long b, long long mod) { 
	long long ret = 0; 
	for (a %= mod, b %= mod; b != 0; b>>=1, a <<= 1, a = a >= mod ? a - mod : a) { 
		if (b&1) {
			ret += a;
			if (ret >= mod)	ret -= mod;
		} 
	} 
	return ret; 
}
unsigned long long mpow(unsigned long long x, unsigned long long y, unsigned long long mod) {
	unsigned long long ret = 1;
	while (y) {
		if (y&1) 
			ret = mul(ret, x, mod);
		y >>= 1, x = mul(x, x, mod);
	}
	return ret % mod;
}

int main() {
	printf("%lld\n", mpow(2, 10, 1000));
	return 0;
}

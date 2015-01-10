#include <cstdio>
#include <stdint.h>

void test() {
	int64_t a,b;
	scanf("%lld %lld", &a, &b);
	printf("%lld\n", a+b);
}

int main() {
	int T;
	scanf("%d", &T);
	while( T --> 0 )
		test();
}

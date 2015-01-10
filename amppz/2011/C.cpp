#include <cstdio>
#include <stdint.h>

int main() {
	uint64_t n,q=1;
	scanf("%llu", &n);
	while( q < n )
		q *= 2;
	puts( n == q ? "TAK" : "NIE" );
}

#include <cstdio>

int main() {
	int n;
	scanf("%d", &n);
	int w = 1;
	long long answer = 0;
	while( 2*w <= n ) {
		answer += 1LL*(w-1)*w/2;
		w *= 2;
	}
	answer += 1LL*(n-w)*(n-w+1)/2;
	printf("%lld\n", answer );
}

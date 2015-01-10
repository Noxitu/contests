#include <cstdio>

int c[2000001];

int main() {
	for( int i = 1; i <= 2000000; i++ )
		c[i] = 0;
		
	int n;
	scanf("%d", &n );
	
	long long answer = -n;
	
	while( n --> 0 ) {
		int a;
		scanf("%d", &a );
		c[a]++;
	}
	
	for( int i = 1; i <= 2000000; i++ )
		for( int j = i; j <= 2000000; j+=i )
			answer += 1LL*c[i]*c[j];
			
	printf("%lld\n", answer );
	return 0;
}
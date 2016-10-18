#include <cstdio>

int main() {
	int n, k;
	scanf("%d %d", &n, &k);
	if( k > n/2 ) {
		puts("NIE");
		return 0;
	}
	for( int i = 1; i <= n/2; i++ ) 
		printf( "%d %d ", n/2+i, i);
	if( n%2 == 1 )
		printf("%d", n);
	printf("\n");
}

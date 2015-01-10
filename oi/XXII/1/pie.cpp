#include <cstdio>

char A[1001][1001], B[1001][1001];
int X[1000000], Y[1000000];

void test() {
	int n, m, a, b, N=0;
	scanf("%d %d %d %d", &n, &m, &a, &b );
	for( int y = 0; y < n; y++ )
		scanf("%s", A+y);
		
	for( int y = 0; y < a; y++ )
		scanf("%s", B+y);
		
	for( int y = 0; y < a; y++ )
		for( int x = 0; x < b; x++ )
			if( B[y][x] == 'x' ) {
				X[N] = x;
				Y[N] = y;
				N++;
			}
			
	for( int y = 0; y < n; y++ )
		for( int x = 0; x < m; x++ )
			if( A[y][x] == 'x' ) {
				for( int i = 0; i < N; i++ ) {
					int xx = x+X[i]-X[0], yy = y+Y[i]-Y[0];
					if( xx < 0 or xx >= m or yy < 0 or yy >= n or A[yy][xx] == '.' ) {
						puts("NIE");
						return;
					}
					A[yy][xx] = '.';
				}
				
			}
	
	puts("TAK");
	return;
}

int main() {
	int T;
	scanf("%d", &T );
	while( T --> 0 )
		test();
}
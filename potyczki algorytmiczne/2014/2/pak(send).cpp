#include <cstdio>
#include <algorithm>
#include <climits>
#include <functional>
#include <cstring>

int n, m;
int A[24], C[100];
int Sols[2];
int Sol[2][1250000][24];

inline int max(const int a, const int b) { return a > b ? a : b; }

inline void FFD() {
	int c[24];
	for( int i = 0; i < m; i++ )
		c[i] = C[i];

	int sol = 0;
	for( int i = 0; i < n; i++ ) {
		for( int j = 0; j <= m; j++ ) {
			if( j == m )
				return;

			if( c[j] >= A[i] ) {
				c[j] -= A[i];
				if( j+1 > sol ) sol = j+1;
				break;
			}
		}
	}

	if( sol < m )
		m = sol;
}

int main() {
	scanf("%d %d", &n, &m );
	for( int i = 0; i < n; i++ ) 
		scanf("%d", A+i );

	for( int i = 0; i < m; i++ )
		scanf("%d", C+i );

	std::sort(A, A+n, std::greater<int>());
	std::sort(C, C+m, std::greater<int>());
	if( m > n )
		m = n;

	while( m > 0 and C[m-1] < A[n-1] )
		m--;

	std::sort(A, A+n);
	FFD();
	std::sort(A, A+n, std::greater<int>());
	FFD();

	int Asum = 0;
	for( int i = 0; i < n; i++ )
		Asum += A[i];

	int M = m, Csum = 0;
	for( m = 0; m < M and Csum < Asum; m++ ) Csum += C[m];

	if( Csum < Asum ) {
		puts("NIE");
		return 0;
	}

	for( ; m <= M; m++ ) {
		Sols[0] = 1;
		for( int j = 0; j < m; j++ )
			Sol[0][0][j] = 0;
		for( int i = 0; i < n; i++ ) {
			int N = Sols[i&1];
			fprintf(stderr, "[n: %d, i:%d, m:%d]\n", N, i, m);
			int& M = Sols[i&1^1];
			int (*Prev)[24] = Sol[i&1];
			int (*Next)[24] = Sol[i&1^1];

			M = 0;
			for( int k = 0; k < N; k++ ) {
				for( int j = 0; j < m; j++ ) {
					if( j > 0 and Prev[k][j] == Prev[k][j-1] )
						continue;

					for( int q = 0; q < m; q++ )
						Next[M][q] = Prev[k][q];

					Next[M][j] += A[i];
					std::sort(Next[M], Next[M]+m, std::greater<int>());


					for( int q = 0; q < m; q++ ) {
						if( Next[M][q] > C[q] ) {
							M--;
							break;
						}
					}

					M++;

					if( M >= 1250000 ) {
						printf("%d\n", m);
						return 0;
					}

				}
			}

		
		}
		if( Sols[n&1] != 0 )
			break;
	}
	int i = n&1;
	fprintf(stderr, "[n: %d]\n", Sols[i]);

	if( Sols[i] == 0 ) {
		puts("NIE");
		return 0;
	}

	for( int k = 0; k < Sols[i]; k++ ) {
		for( int j = 0; j < m; j++ ) {
			if( Sol[i][k][j] == 0 )
				m = j; 
		}
	}

	printf("%d\n", m );
}

	

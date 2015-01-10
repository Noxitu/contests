// 0:31
#include <cstdio>
#include <algorithm>
#include <functional>

typedef std::pair<char, int> solution_t;
int A[24], C[100];
solution_t T[16777216];

int main() {
	int n, m;
	scanf("%d %d", &n, &m );
	for( int i = 0; i < n; i++ )
		scanf("%d", A+i);
	for( int i = 0; i < m; i++ )
		scanf("%d", C+i);
		
	std::sort(C, C+m, std::greater<int>() );
	
	if( m > n )
		m = n;
		
	T[0] = solution_t(1,0);
	for( int S = 1; S < (1<<n); S++ ) {
		T[S] = solution_t(n+1,0);
		for( int i = 0; (1<<i) <= S; i++ ) {
			if( (1<<i) & S ) {
				int P = S ^ (1<<i);
				solution_t Q = T[P];
				if( Q.first == n+1 )
					continue;
				Q.second += A[i];
				if( Q.second > C[Q.first-1] ) {
					Q.first++;
					Q.second = A[i];
					if( Q.second > C[Q.first-1] )
						Q.first = n+1;
				}
				if( Q < T[S] )
					T[S] = Q;
			}
		}
	}
	solution_t solution = T[(1<<n)-1];
	if( solution.first == n+1 ) {
		puts("NIE");
	} else {
		printf("%d\n", solution.first );
	}
	
	return 0;
}
// 0:41

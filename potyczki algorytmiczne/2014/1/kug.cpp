#include <cstdio>

#define INFINITY 1000000000

int T[2001][2001];

inline void swap( int& a, int& b ) {
	register int c = a;
	a = b;
	b = c;
}

int main() {
	int n;
	scanf("%d", &n );

	for( int begin = 1; begin <= n; begin++ )
		for( int end = begin; end <= n; end++ )
			scanf("%d", &T[begin][end] );

	long long answer = 0;
	for( int begin = 1; begin <= n; begin++ ) {
		int best = begin;
		for( int end = begin; end <= n; end++ ) {
			//printf("%d ", T[begin][end] );
			if( T[begin][end] < T[begin][best] )
				best = end;
		}
		//printf("\n	selecting %d [%d][%d]\n", T[begin][best], begin, best );

		answer += T[begin][best];

		for( int i = begin; i < best; i++ )
			if( T[begin][i] < T[i+1][best] )
				T[i+1][best] = T[begin][i];

		for( int i = best+1; i <= n; i++ )
			if( T[begin][i] < T[best+1][i] )
				T[best+1][i] = T[begin][i];
		
	}

	printf("%lld\n", answer );
}

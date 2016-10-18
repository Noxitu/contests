#include <cstdio>

int P[1000000][3];

int main() {
	int n, k, p = 0;
	scanf("%d %d", &n, &k );
	for( int i = 0; i < n; i++ ) {
		int v;
		scanf("%d", &v );
		if( i < k ) {
			P[i][2] = v%2;
			p ^= v%2;
		}
		P[i%k][v%2]++;
	}

	int answer = 0, fix_cost = n;
	for( int i = 0; i < k; i++ ) {
		int fix = P[i][0] < P[i][1] ? 0 : 1;
		answer += P[i][fix];

		if( P[i][2] == fix )
			p ^= 1;

		fix = P[i][fix^1]-P[i][fix];
		if( fix < fix_cost )
			fix_cost = fix;
	}
	if( p == 1 )
		answer += fix_cost;

	printf("%d\n", answer );
}


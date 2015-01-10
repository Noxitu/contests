#include <cstdio>

int next[1000001];
int pos[1000001];

int T[1000000];
int V[1000001];
int N[1000001];

long long scores[1000000];

int main() {
	int n, m;
	scanf("%d %d", &n, &m );
	
	for( int i = 0; i < n; i++ )
		scanf("%d", T+i );
		
	for( int i = 0; i <= n; i++ )
		next[i] = n;
	
	for( int i = 1; i <= m; i++ ) {
		scanf("%d", V+i );
		N[i] = 0;
	}
		
	long long score = 0;
	for( int i = 0; i < n; i++ ) {
		int mov = T[i];
		if( N[mov] == 0 )
			score += V[mov];
		if( N[mov] == 1 )
			score -= V[mov];
			
		if( N[mov] != 0 )
			next[pos[mov]] = i;
			
		pos[mov] = i;
		N[mov]++;
		scores[i] = score;
	}
	
	for( int k = 0; k < n; k++ ) {
		if( scores[k] > score )
			score = scores[k];
	}

	for( int i = 0; i < n; i++ ) {
		int v = V[T[i]];
		int a = i;
		int b = next[a];
		int c = next[b];
		
		for( int j = a; j < b; j++ )
			scores[j] -= v;
		
		for( int j = b; j < c; j++ )
			scores[j] += v;
			
		for( int k = 0; k < n; k++ ) {
			if( scores[k] > score )
				score = scores[k];		
		}
	}
	
	printf("%lld\n", score );
}
		
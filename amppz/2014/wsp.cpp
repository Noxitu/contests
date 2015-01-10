#include <cstdio>
#include <vector>
#include <algorithm>

std::vector<int> V[200001];
int main() {
	int n, m;
	scanf("%d %d", &n, &m );
	while( m --> 0 ) {
		int a, b;
		scanf("%d %d", &a, &b );
		V[a].push_back(b);
		V[b].push_back(a);
	}
	for( int i = 1; i <= n; i++ )
		std::sort( V[i].begin(), V[i].end() );
	
	int q;
	scanf("%d", &q );
	while( q --> 0 ) {
		int c, d, answer = 0;
		scanf("%d %d", &c, &d );
		std::vector<int>::iterator i = V[c].begin(), j = V[d].begin();
		while( i != V[c].end() and j != V[d].end() ) {
			if( *i == *j ) {
				answer++; i++; j++;
			} else if( *i < *j )
				i++;
			else
				j++;
		}
		printf("%d\n", answer );
	}
	
	return 0;
}
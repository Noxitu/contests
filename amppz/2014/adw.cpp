#include <cstdio>
#include <climits>

int end[21], begin[21];
int answer[21][2];

int main() {
	int n, m;
	scanf("%d %d", &n, &m );
	for( int i = 1; i <= m; i++ ) {
		end[i] = INT_MAX;
		begin[i] = 0;
	}
	for( int i = 1; i <= n; i++ ) {
		int a, b, d;
		scanf("%d %d %d", &a, &b, &d );
		if( b < end[d] ) {
			answer[d][0] = i;
			end[d] = b;
		}
		if( a > begin[d] ) {
			answer[d][1] = i;
			begin[d] = a;
		}
	}
	
	for( int i = 1; i <= m; i++ ) {
		if( end[i] < begin[i] )
			printf("TAK %d %d\n", answer[i][0], answer[i][1] );
		else
			printf("NIE\n");
	}
	
	return 0;
}
#include <cstdio>
#include <climits>

int total[65536];
int cheap[65536];
int current[65536];
int price[16];

int main() {
	int n, m;
	scanf("%d %d", &n, &m );
	
	cheap[0] = total[0] = 0;
	for( int S = 1; S < (1<<m); S++ ) {
		cheap[S] = INT_MAX;
		total[S] = 100000000;
	}
		
	while( n --> 0 ) {
		int cost;
		scanf("%d", &cost );
		
		for( int i = 0; i < m; i++ )
			scanf( "%d", price + i );
		
		current[0] = cost; // tymczasowo, bo tak łatwiej
		
		for( int S = 1, i = 0; S < (1<<m); S++ ) {
			if( (2<<i) == S )
				i++;
			
			current[S] = current[S^(1<<i)] + price[i];
			if( current[S] < cheap[S] )
				cheap[S] = current[S];
		}
		
		current[0] = 0; // niepotrzebne
	}

	
	for( int S = 1; S < (1<<m); S++ )
		for( int R = 1; R <= S; R++ ) {
			if( ((~S) bitand R) ) { // jeśli R ma 1 na jakimś bicie na którym S ma 0
				R += ((~S) bitand R) - 1; // skocz do najbliższego R takiego, które ma tam 0
				continue;
			}
			
			int cost = cheap[R] + total[S^R];
			if( cost < total[S] )
				total[S] = cost;
		}
			
	printf("%d\n", total[(1<<m)-1] );
	return 0;
}
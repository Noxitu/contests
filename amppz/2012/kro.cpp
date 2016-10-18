#include <cstdio>
#include <list>
#include <queue>
#include <algorithm>
#include <cmath>

#define INF 2000000000

int patches[2000];
int killed[2001][3];

int main() {
	int n, k;
	scanf("%d %d", &n, &k );

	for( int i = 0; i < n; i++ )
		scanf("%d", patches + i );

	if( k > (n-1)/2 ) {
		k = 0;
		for( int i = 0; i < n; i++ )
			k += patches[i];
		printf("%d\n", k);
		return 0;
	}

	int answer = 0;
	for( int q = 0; q < 3; q++ ) {

		for( int j = 0; j <= k; j++ )
			killed[j][0] = killed[j][1] = killed[j][2] = -INF;

		killed[0][q] = 0;

		for( int i = 0; i < n; i++ ) {
			for( int j = k; j >= 0; j-- ) {

				killed[j][2] = std::max(killed[j][2], killed[j][1]);
				killed[j][1] = killed[j][0];


				if( j > 0 ) {
					killed[j][0] = patches[i] + killed[j-1][2];
					int kk = patches[i] + patches[i>0?i-1:n-1] + killed[j-1][1];
					if( kk > killed[j][0] )
						killed[j][0] = kk;
					kk = killed[j-1][0];
					if( kk > killed[j][0] )
						killed[j][0] = kk;
				} else
					killed[j][0] = -INF;



//				printf("%d  [n=%d][k=%d] = %d, %d, %d\n", q, i, j, killed[j][2], killed[j][1], killed[j][0]);
			}
		}

		if( killed[k][q] > answer )
			answer = killed[k][q];

	}

	printf("%d\n", answer );
}


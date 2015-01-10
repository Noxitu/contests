#include <cstdio>
#include <algorithm>

struct point_t {
	int x, y, i;
};

point_t T[200000];
point_t X[400000];
point_t Y[400000];
int amount1d[400000];
int amount[400000];
int n;

void amount_add(int x) {
	int l = 0, r = 4*n;
	while( l < r ) {
		int m = (l+r)/2;
		if( X[m].x <= x ) {
			amount1d[m]++;
			l = m+1;
		} else {
			r = m;
		}
	}
}

void amount_del(int x) {
	int l = 0, r = 4*n;
	while( l < r ) {
		int m = (l+r)/2;
		if( X[m].x <= x ) {
			amount1d[m]--;
			l = m+1;
		} else {
			r = m;
		}
	}
}

int amount_get(int x) {
	int ret = 0;
	int l = 0, r = 4*n;
	while( l < r ) {
		int m = (l+r)/2;
		if( X[m].x < x ) {
			l = m+1;
		} else {
			ret += amount1d[m];
			r = m;
		}
	}
	return ret;
}

bool compByX(const point_t &a, const point_t &b) { return a.x < b.x; }
bool compByY(const point_t &a, const point_t &b) { return a.y == b.y ? a.x < b.x : a.y < b.y; }

int main() {
	scanf("%d", &n );
	for( int i = 0; i < n; i++ ) {
		int a = 2*i, b = a+1;
		scanf("%d %d %d %d", &T[a].x, &T[b].x, &T[a].y, &T[b].y );
		X[4*i+0].x = X[4*i+1].x = Y[4*i+0].x = Y[4*i+1].x = T[a].x;
		X[4*i+2].x = X[4*i+3].x = Y[4*i+2].x = Y[4*i+3].x = T[b].x-1;
		X[4*i+0].y = X[4*i+2].y = Y[4*i+0].y = Y[4*i+2].y = T[a].y;
		X[4*i+1].y = X[4*i+3].y = Y[4*i+1].y = Y[4*i+3].y = T[b].y-1;
		T[a].i = T[b].i =
		X[4*i+0].i = X[4*i+1].i = X[4*i+2].i = X[4*i+3].i = 
		Y[4*i+0].i = Y[4*i+1].i = Y[4*i+2].i = Y[4*i+3].i = i;
	}

	std::sort( X, X+4*n, compByX );
	std::sort( Y, Y+4*n, compByY );

	for( int i = 0; i < 4*n; i++ ) {
		amount_add(Y[i].x);
	}

	for( int i = 0; i < 4*n; i++ ) {
		printf("%d[%d] ", X[i].x, amount[i] );
	}
	
	printf("\n%d\n", amount_get(4) );

	
}

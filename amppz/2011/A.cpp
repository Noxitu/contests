#include <cstdio>
#include <stack>
#include <cassert>

int T[3030][3030];
bool A[3030][3030]; //pionowo
bool B[3030][3030]; //poziomo
bool V[3030][3030]; //kwadraty
bool X;
int lowest[3030];

struct pair { int x,y; pair(int x, int y) : x(x), y(y) {} };

void test() {
	int w, h;
	scanf("%d %d", &h, &w );
	for( int y = 0; y < h; y++ )
		for( int x = 0; x < w; x++ )
			scanf("%d", &T[y][x] );

	X = false;
	for( int y = 0; y < h-2; y++ )
		for( int x = 0; x < w; x++ ) {
			A[y][x] = T[y][x]-T[y+1][x] == T[y+1][x]-T[y+2][x];
			X |= A[y][x];
		}

	for( int y = 0; y < h; y++ )
		for( int x = 0; x < w-2; x++ ) {
			B[y][x] = T[y][x]-T[y][x+1] == T[y][x+1]-T[y][x+2];
			X |= B[y][x];
		}

	for( int y = 0; y < h-2; y++ )
		for( int x = 0; x < w-2; x++ )
			V[y][x] = A[y][x] and A[y][x+1] and A[y][x+2] and B[y][x] and B[y+1][x] and B[y+2][x];

	int answer = 1;
	for( int x = 0; x < w-2; x++ )
		lowest[x] = -1;
	std::stack<pair> S;
	for( int y = 0; y < h-2; y++ ) {
		while( not S.empty() )
			S.pop();

		S.push(pair(-1,y));
		lowest[w-2] = y;
		for( int x = 0; x < w-1; x++ ) {
			if( not V[y][x] )
				lowest[x] = y;
			pair now(x, lowest[x]);

			while( S.top().y < now.y ) {
				int top_y = S.top().y;
				S.pop();
				int top_x = S.top().x;
				int a = (x-1 - top_x + 2) * (y - top_y + 2);
				if( answer < a )
					answer = a;
			}
			if( S.top().y == now.y )
				S.pop();
			S.push(now);
		}
	}

	if( answer < 2*h )
		for( int x = 0; x < w-1; x++ ) {
			int total = 4;
			for( int y = 0; y < h-2; y++ )
				if( A[y][x] and A[y][x+1] ) {
					total += 2;
					if( answer < total )
						answer = total;
				} else
					total = 4;
		}
	if( answer < 2*w )
		for( int y = 0; y < h-1; y++ ) {
			int total = 4;
			for( int x = 0; x < w-2; x++ )
				if( B[y][x] and B[y+1][x] ) {
					total += 2;
					if( answer < total )
						answer = total;
				} else
					total = 4;
		}
	if( answer < h )
		for( int x = 0; x < w; x++ ) {
			int total = 2;
			for( int y = 0; y < h-2; y++ )
				if( A[y][x]  ) {
					total ++;
					if( answer < total )
						answer = total;
				} else
					total = 2;
		}
	if( answer < w )
		for( int y = 0; y < h; y++ ) {
			int total = 2;
			for( int x = 0; x < w-2; x++ )
				if( B[y][x]) {
					total++;
					if( answer < total )
						answer = total;
				} else
					total = 2;
		}
	if( answer < 4 and w >= 2 and h >= 2 )
		answer = 4;
	if( answer < 2 and ( w >= 2 or h >= 2 ) )
		answer = 2;
	printf("%d\n", answer );
}

int main() {
	int T;
	scanf("%d", &T );
	while( T --> 0 )
		test();
}

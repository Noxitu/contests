#include <cstdio>
#include <algorithm>

bool M[1002][1002];

void rotate_left( int& dx, int& dy ) {
	int _dx = dx;
	dx = -dy;
	dy = _dx;
}

void rotate_right( int& dx, int& dy ) {
	int _dx = dx;
	dx = dy;
	dy = -_dx;
}

typedef void(*rotate_t)(int&, int&);

#define GNUPLOT 

int main() {
	int n, m, f;
	scanf("%d %d %d", &n, &m, &f );
	
	for( int x = 1; x <= n; x++ )
		M[x][0] = M[x][1] = M[x][m+1] = true;
	for( int y = 1; y <= m; y++ )
		M[0][y] = M[n+1][y] = true;
		
	while( f --> 0 ) {
		int x, y;
		scanf("%d %d", &x, &y );
		M[x][y] = M[x+1][y] = M[x+1][y+1] = M[x][y+1] = true;
	}
	
	int x = 1, y = 1, dx = 0, dy = 1;
	rotate_t rotate_first = rotate_left;
	rotate_t rotate_next = rotate_right;
	
	#ifdef GNUPLOT
		fprintf(stderr, "%d %d\n", x, y );
	#endif
	
	do {
		if( dy != -1 or y != 2 )
			rotate_first(dx, dy);
			
		while( M[x+dx][y+dy] )
			rotate_next(dx,dy);
		
		x += dx;
		y += dy;
		M[x][y] = true;
		
		#ifdef GNUPLOT
			fprintf(stderr, "%d %d\n", x, y );
		#endif
		putchar( dx == 0 ? dy == 1 ? 'G' : 'D' : dx == 1 ? 'P' : 'L' );
		
		if( dx == 1 )
			std::swap( rotate_first, rotate_next );
		
		if( x == n and M[1][1] ) {
			for( int x = 1; x <= n; x++ )
				M[x][1] = false;
		}
		
		
	} while( x != 1 or y != 1 );
	
	puts("");
	
	return 0;
}
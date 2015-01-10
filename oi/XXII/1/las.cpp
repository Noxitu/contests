#include <cstdio>

int n;
int T[1000001];
int A[1000000];

void rotate_right(int i) {
	while( A[i] == -1 and T[i] <= 2*T[i+1] ) {
		A[i] = i+1;
		i++;
		if( i == n ) i = 0;
	}
	if( A[i] != -1 )
		return;
		
	while( T[i] > T[i+1] ) {
		A[i] = i;
		if( i == 0 ) i = n;
		i--;
	}
}

void rotate_left(int i) {
	while( A[i] == -1 and 2*T[i] >= T[i+1] ) {
		A[i] = i;
		if( i == 0 ) i = n;
		i--;
	}
	if( A[i] != -1 )
		return;
		
	while( T[i] < T[i+1] ) {
		A[i] = i+1;
		i++;
		if( i == n ) i = 0;
	}
}

int main() {
	scanf("%d", &n );
	for( int i = 0; i < n; i++ ) {
		scanf( "%d", T+i );
		A[i] = -1;
	}
		
	T[n] = T[0];
		
	for( int i = 0; i < n; i++ ) {
		if( A[i] == -1 ) {
			if( 2*T[i] < T[i+1] )
				rotate_right( i );
			if( T[i] > 2*T[i+1] )
				rotate_left( i );
		}
	}
	
	for( int i = 0; i < n; i++ ) {
		if( A[i] == -1 ) {
			if( T[i] < T[i+1] )
				rotate_right( i );
			else
				rotate_left( i );
		}
	}
	
	for( int i = 0; i < n; i++ ) {
		if( A[i] == n )
			A[i] = 0;
		printf("%d ", A[i]+1 );
	}

}
#include <cstdio>

int main() {
	int n, tea = 0;
	static char T[100001];
	scanf("%d %s", &n, T);
	for( int i = 0; i < n-1; i++ ) {
		if( T[i] == 'H' ) tea++;
		else tea--;
	}
	if( n == 1 ) puts("HM");
	else {
		if( tea == 0 ) tea = T[n-2] == 'H' ? -1 : 1;
		if( tea > 0 ) puts("H");
		if( tea < 0 ) puts("M");
	}
}

#include <cstdio>

char D[10001];
int C[256];
int main() {
	scanf("%*d %s", D );
	for( char *d = D; *d; d++ )
		C[*d]++;

	char ret = 'A';
	for( const char *c = "CGT"; *c; c++ )
		if( C[ret] > C[*c] )
			ret = *c;

	for( char *d = D; *d; d++ )
		*d = ret;
	
	printf("%d\n%s\n", C[ret], D );
}


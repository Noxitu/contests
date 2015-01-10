#include <cstdio>
#include <stdint.h>

char T[100001];
int main() {
	scanf("%*d %s", T);
	int now=0, max=0;
	for( int i = 0; T[i]; i++ )
		if( T[i] == 'S' ) {
			if( ++now > max )
				max = now;
		} else
			now = 0;
	printf("%d\n", max );
}

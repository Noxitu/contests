#include <cstdio>
#include <cstdlib>
#include <climits>

void test() {
	int n;
	scanf("%d", &n );

	bool ok = false;
	int A=INT_MAX, B=0, C=INT_MAX, D=0;

	while( n --> 0 ) {
		int a, b, c, d;
		scanf("%d %d %d %d", &a, &b, &c, &d );
		if( a < A ) A = a, ok = false;
		if( b > B ) B = b, ok = false;
		if( c < C ) C = c, ok = false;
		if( d > D ) D = d, ok = false;


		if( a == A and b == B and c == C and d == D )
			ok = true;
	}

	puts( ok ? "TAK" : "NIE" );
}

int main() {
	int T;
	scanf("%d", &T );
	while( T --> 0 )
		test();
}

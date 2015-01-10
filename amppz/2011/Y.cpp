#include <cstdio>
#include <stdint.h>
#include <list>

int main() {
	int n;
	scanf("%d", &n );
	std::list<int> A;
	if( n&1 ) {
		A.push_back(1);
		int b = 2, e = n;
		bool u = true;	
		while( b < e ) {
			if( u ) {
				A.push_front(e--);
				A.push_back(e--);
			} else {
				A.push_back(b++);
				A.push_front(b++);
			}
			u = not u;
		}
	} else {
		bool u = true;
		for( int i = 1; i <= n/2; i++, u = not u ) {
			if( u ) {
				A.push_front(i);
				A.push_back(n-i+1);
			} else {
				A.push_front(n-i+1);
				A.push_back(i);
			}
		}
	}
	for( std::list<int>::iterator it = A.begin(); it != A.end(); it++ )
		printf("%d ", *it );
	printf("\n");
}

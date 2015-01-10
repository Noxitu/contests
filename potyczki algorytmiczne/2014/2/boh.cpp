#include <cstdio>
#include <algorithm>

struct monster_t {
	int d, a, i;

	int operator()() const {
		if( d <= a )
			return -1000000+d;
		else
			return  1000000-a;
	}

};

monster_t Monsters[100000];

bool comp(const monster_t& a, const monster_t& b) { return a() < b(); }

int main() {
	int n;
	long long z;
	scanf("%d %lld", &n, &z );
	for( int i = 0; i < n; i++ ) {
		scanf("%d %d", &Monsters[i].d, &Monsters[i].a );
		Monsters[i].i = i+1;
	}

	std::sort( Monsters, Monsters+n, comp );

	for( int i = 0; i < n; i++ ) {
		register monster_t& M = Monsters[i];

		if( z <= M.d ) {
			puts("NIE");
			return 0;
		}

		z += M.a - M.d;
	}

	printf("TAK\n");
	for( int i = 0; i < n; i++ )
		printf("%d ", Monsters[i].i ); 
	printf("\n");

	return 0;
}





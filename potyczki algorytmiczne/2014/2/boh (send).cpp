#include <cstdio>
#include <algorithm>

int Answer[100000];

struct monster_t {
	int d, a, i;
	inline void operator=(const monster_t& o) {
		d = o.d;
		a = o.a;
		i = o.i;
	}
};

monster_t Monsters[100000];

bool compByD(const monster_t& a, const monster_t& b) { return a.d < b.d; }
bool compByA(const monster_t& a, const monster_t& b) { return a.a < b.a; }

int main() {
	int n;
	long long lifeCan;
	scanf("%d %lld", &n, &lifeCan );
	for( int i = 0; i < n; i++ ) {
		scanf("%d %d", &Monsters[i].d, &Monsters[i].a );
		Monsters[i].i = i+1;
	}

	std::sort( Monsters, Monsters+n, compByD );

	int m = 0, *answer = Answer;

	for( int i = 0; i < n; i++ ) {
		register monster_t& M = Monsters[i];

		if( lifeCan <= M.d ) {
			puts("NIE");
			return 0;
		}

		if( M.d <= M.a ) {
			lifeCan += M.a - M.d;
			*(answer++) = M.i;
		} else {
			Monsters[m++] = M;
		}
	}

	std::sort( Monsters, Monsters+m, compByA );

	long long lifeNeed = 0;
	answer = Answer+n;

	for( int i = 0; i < m; i++ ) {
		register monster_t& M = Monsters[i];
		
		if( lifeNeed <= M.a )
			lifeNeed = M.a+1;

		lifeNeed += M.d - M.a;
		*(--answer) = M.i;
	}

	if( lifeCan < lifeNeed ) {
		puts("NIE");
		return 0;
	}

	printf("TAK\n");
	for( int i = 0; i < n; i++ )
		printf("%d ", Answer[i]);
	printf("\n");

	return 0;
}





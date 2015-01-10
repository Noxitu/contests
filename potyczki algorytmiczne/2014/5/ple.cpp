#include <cstdio>
#include <algorithm>
#include <set>
#include <map>
#include <climits>
#include <cassert>

struct terrain_t;
struct range_t {
	int a, b;
	terrain_t* t;
	range_t() {}
	range_t(int v) : a(v), b(v), t(NULL) {}
	void absorb(range_t& o) {
		if( o.a < a ) a = o.a;
		if( o.b > b ) b = o.b;
	}
};

bool operator<(const range_t &a, const range_t &b) {
	return a.a < b.a;
}

enum {
	BEFORE = 0,
	AFTER = 1,
	DELETED = 2
};

struct terrain_t {
	range_t x, y;
	char state;
	void absorb(terrain_t& o) {
		x.absorb(o.x);
		y.absorb(o.y);
		o.state = DELETED;
	} 
};

bool operator<(const terrain_t &a, const terrain_t &b) {
	if( a.state != b.state ) return a.state < b.state;
	if( a.x.a != b.x.a ) return a.x.a < b.x.a;
	return a.x.b < b.x.b;
}

std::multimap<int, terrain_t*> queue;
std::set<range_t> ranges;

terrain_t T[100000];

int main() {
	int n;
	scanf("%d", &n );
	for( int i = 0; i < n; i++ ) {
		scanf("%d %d %d %d", &T[i].x.a, &T[i].x.b, &T[i].y.a, &T[i].y.b );
		T[i].x.t = T[i].y.t = T+i;
	}

	ranges.insert(range_t(INT_MIN));
	ranges.insert(range_t(INT_MAX));

	int territoriesLeft = n;
	int direction = false;
	while( true ) {
		direction = not direction;
		int beginTerritoriesLeft = territoriesLeft;
		for( int i = 0; i < n; i++ ) 
			if( T[i].state != DELETED ) {
				queue.insert( std::make_pair(2*(direction ? T[i].y.a : -T[i].y.b), T+i) );
				T[i].state = BEFORE;
			}

		while( not queue.empty() ) {
			range_t& x = queue.begin()->second->x;
			queue.erase(queue.begin());
			switch(x.t->state) {
				case DELETED:
					break;

				case BEFORE: {
					std::set<range_t>::iterator it = ranges.lower_bound(range_t(x.b));
					it--;
					while( it->b > x.a ) {
						x.t->absorb(*it->t);
						territoriesLeft--;
						std::set<range_t>::iterator rit = it;
						it--;
						ranges.erase(rit);
					}

					ranges.insert(x);
					queue.insert( std::make_pair(2*(direction ? x.t->y.b : -x.t->y.a)-1, x.t) );
					x.t->state = AFTER;
				}	break;

				case AFTER:
					ranges.erase(x);
					break;
			}
		}

		if( beginTerritoriesLeft == territoriesLeft )
			break;
	}

	std::sort( T, T+n );
	printf("%d\n", territoriesLeft );
	for( int i = 0; i < territoriesLeft; i++ )
		printf("%d %d %d %d\n", T[i].x.a, T[i].x.b, T[i].y.a, T[i].y.b);
	
	return 0;
}

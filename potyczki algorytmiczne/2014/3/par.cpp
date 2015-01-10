#include <cstdio>
#include <algorithm>
#include <map>
#include <climits>
#include <cstdlib>

struct car_t {
	int x1, x2, w, i;
};

bool compByX2(const car_t& a, const car_t& b) { return a.x2 < b.x2; }

car_t cars[50000];

void test() {
	int n, w;
	scanf("%d %d", &n, &w );
	for( int i = 0; i < n; i++ ) {
		int a, b, c, d;
		scanf("%d %d %d %d", &a, &b, &c, &d );
		cars[i].x1 = std::min(a,c);
		cars[i].w = abs(b-d);
	}
	for( int i = 0; i < n; i++ ) {
		int a, c;
		scanf("%d %*s %d %*s", &a, &c );
		cars[i].x2 = std::min(a,c);
	}
		
	std::sort(cars, cars+n, compByX2);
	std::map<int, int> Cross;

	Cross[INT_MIN] = INT_MAX;
	Cross[INT_MAX] = 0;

	for( int i = 0; i < n; i++ ) {
		int cross = Cross.lower_bound(cars[i].x1)->second;
		if( cars[i].w+cross > w ) {
			puts("NIE");
			return;
		}
		if( cars[i].w > cross ) {
			Cross[cars[i].x1] = cars[i].w;
			std::map<int, int>::iterator it = Cross.find(cars[i].x1);
			--it;
			while( it->second < cars[i].w ) {
				std::map<int, int>::iterator rit = it;
				--it;
				Cross.erase(rit); 
			}
		}
	}

	puts("TAK");
}

int main() {
	int T;
	scanf("%d", &T );
	while( T --> 0 )
		test();
}

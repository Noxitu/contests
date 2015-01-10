#include <cstdio>
#include <set>

int main() {
	int n, k, v;
	scanf("%d %d", &n, &k );

	std::multiset<int> got, need, cost;
	long long total = 0;

	for( int i = 0; i < n; i++ ) {
		scanf("%d", &v );
		got.insert(v);
	}
		
	for( int i = 0; i < n; i++ ) {
		scanf("%d", &v );
		need.insert(v);
	}

	for( std::set<int>::reverse_iterator it = need.rbegin(); it != need.rend(); it++ ) {
		std::set<int>::iterator it2 = got.lower_bound(*it);
		if( it2 == got.end() ) {
			printf("%d !!\n", *it );
			total += *it;
			k--;
		} else {
			printf("%d -- %d\n", *it, *it2 );
			total += *it2;
			cost.insert( *it2 - *it );
			got.erase( it2 );
		}
	}

	for( std::set<int>::reverse_iterator it = cost.rbegin(); it != cost.rend() and k > 0; it++, k-- ) {
		printf("%d: %d\n", k, *it );
		total -= *it;
	}

	printf("%lld\n", total );
}

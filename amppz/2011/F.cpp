#include <cstdio>
#include <queue>
#include <set>

std::priority_queue<int> counts;
std::multiset<int> capacities;

bool try_remove(int value) {
	std::multiset<int>::iterator it = capacities.lower_bound(value);
	if( it == capacities.end() )
		return false;
	capacities.erase(it);
	return true;
}

int main() {
	int n, k;

	scanf("%d %d", &n, &k );
	int answer = n;

	while( n --> 0 ) {
		int val;
		scanf("%d", &val );
		counts.push( val );
	}
	while( k --> 0 ) {
		int val;
		scanf("%d", &val );
		capacities.insert( val );
	}

	while( not counts.empty() ) {
		int val = counts.top();
		if( not try_remove(5*val) ) {
			if( not try_remove(3*val) or not try_remove(2*val) ) {
				puts("NIE");
				return 0;
			}
			answer++;
		}
		counts.pop();
	}
	printf("%d\n", answer);
}

#include <cstdio>
#include <list>
#include <map>
#include <queue>
#include <algorithm>
#include <cmath>
#include <climits>

struct island_t;

struct edge_t {
	int cost;
	island_t *to;
	edge_t(int cost, island_t* to) : cost(cost), to(to) {}
};

struct island_t : public std::list<edge_t> {
	long long answer;
	std::map<int, int> traps;
	std::map<long long, long long> allowed;
};

island_t islands[100001];

int main() {
	int n, m, k;
	scanf("%d %d", &n, &m );
	while( m --> 0 ) {
		int a, b, d;
		scanf("%d %d %d", &a, &b, &d );
		islands[a].push_back( edge_t( d, islands+b ) );
	}
	
	scanf("%d", &k );
	
	while( k --> 0 ) {
		int w, s, k;
		scanf("%d %d %d", &w, &s, &k );
		islands[w].traps[s] = std::max(islands[w].traps[s], k);
	}
	
	for( int i = 1; i <= n; i++ ) {
		int since = 0;
		for( std::map<int, int>::iterator it = islands[i].traps.begin(); it != islands[i].traps.end(); it++ ) {
			int until = it->first-1;
			if( since < until ) 
				islands[i].allowed[since] = until;
				
			since = it->second + 1;
			
		}
		islands[i].allowed[since] = 1000000000000000LL;
		islands[i].traps.clear();
		islands[i].answer = INT_MAX;
	}
		
	std::priority_queue<std::pair<long long, island_t*> > Q;
	
	Q.push( std::make_pair( -1, islands+1 ) );
	islands[1].answer = 1;
	
	while( not Q.empty() ) {
		long long when = -Q.top().first;
		island_t& v = *Q.top().second;
		Q.pop();
		
		while( not v.allowed.empty() and v.allowed.begin()->second < when ) {
			//printf("\tIsland %d at time %d (too late >%d)\n", &v - islands, when, v.allowed.begin()->second );
			v.allowed.erase( v.allowed.begin() );
		}
		
		if( v.allowed.empty() or v.allowed.begin()->first > when )
			continue;
			
		if( when < v.answer )
			v.answer = when;
		
		long long wait = v.allowed.begin()->second - when;
		
		//printf("Island %d at time %d (+%d)\n", &v - islands, when, wait );
		
		
		
		for( island_t::iterator it = v.begin(); it != v.end(); it++ ) {
			std::map<long long, long long>::iterator jt = it->to->allowed.lower_bound(when+it->cost);
			if( jt != it->to->allowed.begin() ) {
				std::map<long long, long long>::iterator jt2 = jt;
				jt--;
				if( jt->second < when+it->cost )
					jt = jt2;
			}
			//printf(":: first free time at island %d: %d\n", it->to - islands, jt->first );
			while( jt != it->to->allowed.end() and jt->first < when+it->cost+wait ) {
				long long t = std::max( jt->first, when+it->cost );
				Q.push( std::make_pair( -t, it->to ) );
				jt++;
			}
			
		}
		
		v.allowed.erase( v.allowed.begin() );
		
	}
	
	if( islands[n].answer == INT_MAX )
		puts("NIE");
	else
		printf("%lld\n", islands[n].answer-1 );
		
}

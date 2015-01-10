#include <cstdio>
#include <list>
#include <queue>
#include <algorithm>
#include <climits>
#include <stack>
#include <cassert>

struct Animal : std::list<Animal*> {
	Animal *matched;
	Animal *prev;
	Animal::iterator prev_it;
	int layer;
	bool visited;
};
Animal T[100001];

int cows, bulls;

void match(Animal *who) {
	Animal *prev = who->prev, *next = who;
	while( true ) {
		prev->erase( next->prev_it );
		next->push_back(prev);
		
		if( not prev->matched )
			break;
			
		next = prev;
		prev = prev->prev;
	}
	
	who->matched = prev;
	prev->matched = who;
}

bool bfs() {
	std::queue<Animal*> Q;
	
	for( int j = 1; j <= cows+bulls; j++ ) {
		T[j].visited = false;
		T[j].layer = 0;
		if( not T[j].matched and j <= cows )
			Q.push( T+j );
	}
	
	int best_layer = INT_MAX;
		
	while( not Q.empty() ) {
		Animal *v = Q.front(); Q.pop();
		
		if( v->layer == best_layer )
			return true;
		
		for( Animal::iterator it = v->begin(); it != v->end(); it++ ) {
			if( not (*it)->visited ) {
				(*it)->layer = v->layer+1;
				(*it)->visited = true;
				
				if( not (*it)->matched ) {
					best_layer = (*it)->layer;
				}
				
				Q.push(*it);
			}
		}
	}
	
	return false;
}

int dfss() {
	std::stack<Animal*> Q;
	for( int j = 1; j <= cows+bulls; j++ ) {
		T[j].prev = 0;	
		T[j].visited = false;
		if( not T[j].matched and j <= cows ) {
			Q.push( T+j );
		}
	}
	
	while( not Q.empty() ) {
		Animal *v = Q.top(); Q.pop();
		if( v->visited )
			continue;
		
		v->visited = true;
		
		for( Animal::iterator it = v->begin(); it != v->end(); it++ ) {
			if( (*it)->layer == v->layer+1 and not (*it)->visited ) {
				(*it)->prev = v;
				(*it)->prev_it = it;
				
				if( not (*it)->matched ) {
					(*it)->visited = true;
					while( not Q.empty() and Q.top()->matched )
						Q.pop();
						
					break;
				}
				
				Q.push(*it);
			}
		}
	}
	
	int m = 0;
	for( int j = 1; j <= bulls; j++ )
		if( not T[cows+j].matched and T[cows+j].visited ) {
			match(T+cows+j);
			m++;
		}
	return m;
}

int main() {
	int m;
	scanf("%d %d %d", &cows, &bulls, &m );
	while( m --> 0 ) {
		int c, b;
		scanf("%d %d", &c, &b);
		T[c].push_back(T+cows+b);
	}
	
	m = 0;
	while( bfs() ) {
		m += dfss();
	}
	
	printf("%d\n", m );
} 
#include <cstdio>
#include <climits>
#include <queue>

struct Vertex {
	int x, y;
	std::vector< Vertex* > right, down;
};

Vertex T[100001];

int main() {
	int n, m, k;

	scanf("%d %d %d", &n, &m, &k );
	
	// Load verticles, flip them arount OX
	for( int i = 1; i <= n; i++ ) {
		scanf("%d %d", &T[i].x, &T[i].y );
		T[i].y = -T[i].y;
	}

	// Load edges - we have directed graph without cycles with no more then 2 outgoing edges
	while( m --> 0 ) {
		int from, to;
		scanf("%d %d", &from, &to );
		if( T[from].x > T[to].x or T[from].y > T[to].y )
			std::swap( from, to );

		if( T[from].x == T[to].x )
			T[from].down.push_back(T+to);
		else
			T[from].right.push_back(T+to);
	}

	// If any vertex has only one outgoing edge - double it
	for( int i = 1; i < n; i++ ) {
		if( T[i].down.empty() )
			T[i].down.push_back(T[i].right[0]);
		if( T[i].right.empty() )
			T[i].right.push_back(T[i].down[0]);
	}

	// Create table of descendants at levels 2**n
	for( int step = 0; T[1].right.size() > step or T[1].down.size() > step; step++ ) {
		for( int i = 1; i <= n; i++ ) {
			if( T[i].down.size() > step and T[i].down[step]->down.size() > step )
				T[i].down.push_back( T[i].down[step]->down[step] );
			if( T[i].right.size() > step and T[i].right[step]->right.size() > step )
				T[i].right.push_back( T[i].right[step]->right[step] );
		}
	}

	// Parse queries
	while( k --> 0 ) {
		int p, q;
		scanf("%d %d", &p, &q );
		if( T[p].x > T[q].x or T[p].y > T[q].y )
			std::swap(p,q);
		if( T[p].x > T[q].x or T[p].y > T[q].y ) {
			puts("NIE");
			continue;
		}

		//Check path down
		Vertex *v = T+p;
		int i = v->down.size()-1;
		for( i; i >= 0; i-- ) {
			while( i >= v->down.size() ) i--;
			Vertex *u = v->down[i];
			if( u->y < T[q].y )
				v = u;
		}
		v = v->down[0];
		if( v->x > T[q].x ) {
			puts("NIE");
			continue;
		}

		//Check path right
		v = T+p;
		i = v->right.size()-1;
		for( i; i >= 0; i-- ) {
			while( i >= v->right.size() ) i--;
			Vertex *u = v->right[i];
			if( u->x < T[q].x )
				v = u;
		}
		v = v->right[0];
		if( v->y > T[q].y ) {
			puts("NIE");
			continue;
		}
		puts("TAK");
	}
}

#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>

struct Vertex;
struct Edge {
	Vertex *a, *b;
	int cost;
};
struct Question {
	Vertex *a, *b;
	int limit;
	int id;
	bool answer;
};

struct Vertex : std::vector< std::pair<Vertex*, int> > {
	bool visited;
	int nearest;
	
	Vertex *parent;
	int rank; // wysokość drzewa
	void clear() {
		parent = NULL;
		rank = 0;
	}
	Vertex* Find() {
		if( parent == NULL )
			return this;
		parent = parent->Find();
		return parent;
	}
	void Union( Vertex *other ) {
		Vertex *self = this->Find();
		other = other->Find();

		if( self == other )
			return;
			
		if( self->rank == other->rank ) {
			other->parent = self;
			self->rank++;
		} else if( self->rank > other->rank ) {
			other->parent = self;
		} else {
			self->parent = other;
		}
	}
};

bool edgeCost( const Edge& a, const Edge& b ) { return a.cost < b.cost; }
bool questionLimit( const Question& a, const Question& b ) { return a.limit < b.limit; }
bool questionId( const Question& a, const Question& b ) { return a.id < b.id; }

Vertex V[200001];
Edge E[200000];
Question qs[200000];

int main() {
	int n, s, m;
	scanf("%d %d %d", &n, &s, &m );
	
	std::priority_queue< std::pair<int, Vertex*> > Q;
	
	for( int i = 1; i <= n; i++ ) {
		V[i].visited = false;
		V[i].clear();
	}
	
	
	while( s --> 0 ) {
		int c;
		scanf("%d", &c );
		Q.push( std::make_pair(0, V+c) );
	}
	
	for( int j = 0; j < m; j++ ) {
		int u, v, d;
		scanf("%d %d %d", &u, &v, &d );
		E[j].a = V+u;
		E[j].b = V+v;
		E[j].cost = d;
		V[u].push_back( std::make_pair(V+v,d) );
		V[v].push_back( std::make_pair(V+u,d) );
	}
	
	int q;
	scanf("%d", &q );
	for( int k = 0; k < q; k++ ) {
		int a, b, c;
		scanf("%d %d %d", &a, &b, &c );
		qs[k].a = V+a;
		qs[k].b = V+b;
		qs[k].limit = c;
		qs[k].id = k;
	}
	
	while( not Q.empty() ) {
		int d = -Q.top().first;
		Vertex *v = Q.top().second;
		Q.pop();
		
		if( v->visited )
			continue;
			
		v->visited = true;
		v->nearest = d;
			
		for( Vertex::iterator it = v->begin(); it != v->end(); it++ )
			Q.push( std::make_pair(-d-it->second, it->first) );
	}
	
	for( int i = 0; i < m; i++ )
		E[i].cost += E[i].a->nearest + E[i].b->nearest;
		
	std::sort( E, E+m, edgeCost );
	std::sort( qs, qs+q, questionLimit );
	
	for( int i = 0, j = 0; i < q; i++ ) {
		for( ;j < m and E[j].cost <= qs[i].limit; j++ )
			E[j].a->Union( E[j].b );
			
		qs[i].answer = (qs[i].a->Find() == qs[i].b->Find() );
	}
	
	std::sort( qs, qs+q, questionId );
	for( int i = 0; i < q; i++ )
		puts( qs[i].answer ? "TAK" : "NIE" );
		
	return 0;
}
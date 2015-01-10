#include <cstdio>
#include <vector>
#include <queue>
#include <climits>
// czas kodowania: 36min

struct vertex_t : std::vector<vertex_t*> {
	int d;
	vertex_t *prev;
};

vertex_t V[300001];
int A[300001], B[300001], D[300001];

int n, m;

void bfs(vertex_t *s) {
	for( int i = 1; i <= n; i++ )
		V[i].d = -1;
		
	std::queue<vertex_t*> Q;
	Q.push(s);
	s->d = 0;
	s->prev = s;
	
	while( not Q.empty() ) {
		vertex_t *v = Q.front();
		Q.pop();
		
		for( vertex_t::iterator it = v->begin(); it != v->end(); it++ ) {
			if( (*it)->d == -1 ) {
				(*it)->d = v->d + 1;
				(*it)->prev = v;
				Q.push(*it);
			}
		}
	}
}

void farthest( int &best, int &best_i ) {
	best=0;
	best_i=1;
	
	for( int i = 1; i <= m; i++ ) {
		int val = V[A[i]].d + V[B[i]].d - D[i];
		if( val > best ) {
			best = val;
			best_i = i;
		}
	}
	
	best = best >= 0 ? (best+1)/2 : 0;
	
}

void test() {
	scanf("%d %d", &n, &m );
	for( int i = 1; i <= n; i++ ) {
		V[i].clear();
	}
	
	for( int i = 1; i < n; i++ ) {
		int a, b;
		scanf("%d %d", &a, &b );
		V[a].push_back( V+b );
		V[b].push_back( V+a );
	}
	
	for( int i = 1; i <= m; i++ )
		scanf("%d %d %d", A+i, B+i, D+i );
		
	int best, best_i;
	
	bfs(V+1);
	farthest( best, best_i );

	vertex_t *a = V+A[best_i], *b = V+B[best_i];
	
	while( a->d > b->d ) a = a->prev;
	while( b->d > a->d ) b = b->prev;
	while( a != b ) {
		a = a->prev;
		b = b->prev;
	}

	while( a->d > best )
		a = a->prev;
	
	bfs(a);
	farthest( best, best_i );
	
	if( best > 0 )
		printf("NIE\n");
	else
		printf("TAK %d\n", a-V );
	
}
	
int main() {
	int T;
	scanf("%d", &T );
	while( T --> 0 )
		test();
	
	return 0;
}


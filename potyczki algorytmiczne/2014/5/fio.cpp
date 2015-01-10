#include <cstdio>
#include <algorithm>

inline void swap(int& a, int& b) { int c = a; a = b; b = c; }
inline int min(int a, int b) { return a < b ? a : b; }
inline int max(int a, int b) { return a > b ? a : b; }

struct vertex_t {
	int parent[20];
	int join;
	int level;
	int mass;
};

struct join_t {
	int from, to;
};

struct react_t {
	int from, to;
	int join;
};

bool compByJoin(const react_t& a, const react_t& b) {
	return a.join < b.join;
}

vertex_t V[200001];
join_t Joins[200000];
react_t Reacts[500000];

int goUp(int from, int amount) {
	for( int i = 0; (1<<i) <= amount; i++ )
		if( amount & (1<<i) )
			from = V[from].parent[i];
	return from;
}

int main() {
	int n, m, k;
	scanf("%d %d %d", &n, &m, &k );

	for( int i = 1; i <= n; i++ ) {
		scanf("%d", &V[i].mass);
		V[i].join = -1;
		V[i].level = 0;
		V[i].parent[0] = i;
	}

	for( int i = 0; i < m; i++ ) {
		scanf("%d %d", &Joins[i].from, &Joins[i].to ); 
		V[Joins[i].from].join = i;
		V[Joins[i].from].parent[0] = Joins[i].to;
	}

	int height = 0;
	for( int i = m-1; i >= 0; i-- ) {
		join_t& j = Joins[i];
		V[j.from].level = V[j.to].level+1;
		if( V[j.from].level > height )
			height = V[j.from].level;
	}

	int I = 0;
	for( I = 0; (1 << I) < height; I++ ) {
		for( int j = 1; j <= n; j++ )
			V[j].parent[I+1] = V[ V[j].parent[I] ].parent[I];
	}

	for( int i = 0; i < k; i++ ) {
		react_t& r = Reacts[i];
		scanf("%d %d", &r.from, &r.to );
		if( V[r.from].parent[I] != V[r.to].parent[I] ) {
			r.join = -1;
			continue;
		}

		int a = r.from, b = r.to;

		if( V[a].level < V[b].level )
			swap( a, b );

		int c = goUp(a, V[a].level-V[b].level);

		if( c == b ) {
			a = goUp(a, V[a].level-V[b].level-1 );
			r.join = V[a].join;
			continue;
		}

		a = c;
		for( int i = I; i >= 0; i-- ) {
			if( V[a].parent[i] != V[b].parent[i] ) {
				a = V[a].parent[i];
				b = V[b].parent[i];
			}
		}

		r.join = max(V[a].join, V[b].join);
	}


	std::stable_sort( Reacts, Reacts+k, compByJoin );

	long long answer = 0;
	for( int i = 0; i < k; i++ ) {
		react_t& r = Reacts[i];
		if( r.join == -1 )
			continue;


		int val = min( V[r.from].mass, V[r.to].mass );
		answer += val;
		V[r.from].mass -= val;
		V[r.to].mass -= val;
	}

	printf("%lld\n", 2*answer );
}

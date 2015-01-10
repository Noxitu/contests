#include <cstdio>
#include <algorithm>
#include <stack>

struct Vertex {
	char bit;
	int visited;
	int count;
	int j; // kmp field
	int mem[2];
	Vertex *parent;
	Vertex *previous;
};

Vertex V[1000000];

char P[100001];
int KMP[100001];
int cycle_length = 1;
void prepare_kmp(int n) {
	KMP[0] = 0;
	for( int i = 1, j = 0; i <= n; i++ ) {
		if( i < n and P[i] != P[j] )
			cycle_length = i+1;
		while( j and P[i] != P[j] )
			j = KMP[j-1];
		if( P[i] == P[j] )
			j++;
		KMP[i] = j;
	}

//	printf("%s\ncycle_length: %d\n", P, cycle_length);
}

void calculate_cycle(Vertex *v, int n) {
	bool once = false;
	for( int j = 0; ; v = v->previous) {
//		printf("calculate_cycle(%d)\n", v-V);
		while( j and v->bit != P[j] )
			j = KMP[j-1];
		if( v->bit == P[j] )
			j++;
		if( v->j >= j ) 
			break;
		if( v->j != -1 and (j-v->j)%cycle_length == 0) {
			j += (n-j)/cycle_length*cycle_length;
			while( j+cycle_length < n )
				j += cycle_length;
		}
		v->j = j;
	}
}

int main() {
	int a, c, k, m, n;
	scanf("%d %d %d %d %d %s", &a, &c, &k, &m, &n, P );
	std::reverse(P, P+n);
	prepare_kmp(n);
	for( int z = 0; z < m; z++ ) {
		V[z].j = -1;
		V[z].mem[0] = V[z].mem[1] = -1;
		V[z].bit = z < m/2 ? '0' : '1';
		V[z].parent = V + (1LL*a*z+c)/k % m;
		V[z].parent->count++;
	}

	std::stack<Vertex*> S;

	for( int i = 0; i < m; i++ ) {
		Vertex *v = V+i;
		while( not v->visited ) {
			S.push(v);
			v->visited = i+1;
			v = v->parent;
			v->parent->previous = v;
		}
		if( v->visited == i+1 ) {
			v->parent->previous = v;
			calculate_cycle(v, n);
		}
		while( not S.empty() ) {
			v = S.top();
			S.pop();
			if( v->parent->mem[v->bit-'0'] == -1 ) {
				int j = v->parent->j;
				while( j and v->bit != P[j] ) {
					j = KMP[j-1];
				}
				if( v->bit == P[j] )
					j++;
				v->j = j;
				v->parent->mem[v->bit-'0'] = j;
			} else
				v-> j = v->parent->mem[v->bit-'0'];
		}
	}

//	for( int i = 0; i < m; i++ )
//		printf("%d[label=\"%d %d %d\"]\n%d -> %d\n", i, i, V[i].j, V[i].count, i, V[i].parent -V );

	int sum = 0;
	for( int i = 0; i < m; i++ )
		if( V[i].j == n )
			sum += V[i].count;

	printf("%d\n", sum );
}

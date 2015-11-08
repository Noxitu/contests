#include <cstdio>

int v[1000001], p[1000001], q[1000001], d[1000001];
long long s[1000001], w[1000001];

struct best_t {
	long long w;
	int d;
	int a, b;
	best_t(int a, int b) : a(a), b(b) {
		//printf("best_t(%d,%d)\n", a, b);
		d = b-a+1;
		w = s[b]-s[a-1];
	}
	bool operator <(const best_t& o) const {
		if( w*o.d == o.w*d ) {
			if( d == o.d )
				return a > o.a;
			return d < o.d;
		} 

		return w*o.d < o.w*d;
	}
};

void test() {
	int N, L;
	scanf("%d %d", &N, &L );
	s[0] = 0;

	for( int i = 1; i <= N; i++ ) {
		scanf("%d", v+i );
		s[i] = s[i-1] + v[i];
	}

	/*
	 * Algorytm wyszukiwania podlancuchow o najwiekszej sredniej
	 *  dla punktu i taki podciag konczy sie w p[i], ma sume s[i] oraz dlugosc d[i]
     */ 
	for( int i = N; i >= 1; i-- ) {
		p[i] = i;
		w[i] = v[i];
		d[i] = 1;
		//printf("\t\tstart for %d; (%d, %lld, %d)\n", i, p[i], w[i], d[i]);
		//printf("values for p[i]+1 (xx, %lld, %d)\n", w[p[i]+1], d[p[i]+1]);
		while( p[i] < N and w[i]*d[p[i]+1] <= w[p[i]+1]*d[i] ) {
			//puts("step");
			w[i] += w[p[i]+1];
			d[i] += d[p[i]+1];
			p[i] = p[p[i]+1];
		}
	}

	/*
	 * Podlancuch X wyznaczony przez identyczne wartosci q[] ma wlasnosc:
	 *   dla kazdego prefiksu P i sufiksu S: PS = X => srednia P <= srednia X <= srednia S
	 * Podlancuch o tej wlasnosci nazywamy ukośnym w prawo
	 * Ponadto srednie kolejnych tak wyznaczonych podciagow sa ciagiem malejacym
	 */
	for( int i = 1, qq = 0; i <= N; i++ ) {
		if( qq < i ) {
			qq = p[i];
		}
		q[i] = qq;
	}

	//for( int i = 1; i <= N; i++ ) {
	//	printf("%3d %3d %3d\n", v[i], p[i], q[i] );
	//}

	best_t best(1,L);
	for( int i = 1; i+L-1 <= N; i++ ) {

		// Podlancuch dlugosci L
		int end = i+L-1;
		while( true ) {
			best_t one(i, end);
			if( best < one )
				best = one;

			if( end < N )
				end = p[end+1];
			else
				break;
		}

	}
	printf("%d %d\n", best.a, best.b);
}

int main() {
	int T;
	scanf("%d", &T );
	while( T --> 0 )
		test();
}

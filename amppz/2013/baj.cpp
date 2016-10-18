#include <cstdio>

struct V {
	V *parent;
	int rank;
	bool empty;
	V() : parent(NULL), rank(0), empty(true) {}
	void clear() {
		parent = NULL;
		rank = 0;
	}
	V* Find() {
		if( parent == NULL )
			return this;
		parent = parent->Find();
		return parent;
	}
	void Union( V *other ) {
		V *self = this->Find();
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

V& operator or(V& a, V& b) {
	if( a.empty )
		return b;
	return a;
}

V T[1502][1502][2];

int main() {
	int n, k;
	scanf("%d %d", &n, &k);
	bool answer = true;

	// Ograniczamy rozwazany teren zamykajac wszystkie ulice, ktore pozwolilyby go opuscic
	T[0][0][0].empty = false;
	for( int i = 1; i <= n; i++ )
		for( int j = 0; j <= n; j += n ) {
			T[i][j][1].empty = T[j][i][0].empty = false;
			T[0][0][0].Union( &T[i][j][1] );
			T[0][0][0].Union( &T[j][i][0] );
		}

	while( k --> 0 ) {
		int x, y; char d;

		// Wczytujemy odpowiednia ulice
		scanf(answer ? "%d %d %c %*d %*d %*s" : "%*d %*d %*s %d %d %c", &x, &y, &d );
		d = ( d == 'N');

		// Szukamy po obu stronach ulicy zablokowanych ulic
		V& left = d ? (T[x-1][y][1] || T[x-1][y][0] || T[x-1][y+1][0]) : (T[x][y][1] || T[x][y+1][0] || T[x+1][y][1]);
		V& right = d ? (T[x][y][0] || T[x+1][y][1] || T[x][y+1][0]) : (T[x][y-1][1] || T[x][y-1][0] || T[x+1][y-1][1]);

		answer = left.empty || right.empty || left.Find() != right.Find();
		puts( answer ? "TAK" : "NIE" );

		T[x][y][d].empty = false;
		if( not left.empty )
			T[x][y][d].Union( &left );
		if( not right.empty )
			T[x][y][d].Union( &right );
	}
}

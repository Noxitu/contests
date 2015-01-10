#include <cstdio>

#define Vector Point

int n;
struct Point {
	int x, y, z;
	void scan() {
		n--;
		scanf("%d %d %d", &x, &y, &z );
	}
	bool operator==( const Point& other ) const {
		return x == other.x and y == other.y and z == other.z;
	}
	Vector operator-(const Point& other ) const {
		Point p = {x-other.x, y-other.y, z-other.z};
		return p;
	}
};


int main() {
	scanf("%d", &n );
	Point P;
	P.scan();

	Point Q = P;
	while( n and P == Q )
		Q.scan();

	Vector K = Q-P;

	long long A=0, B=0, C=0, D=0;
		Point R;
	while( n and A == 0 and B == 0 and C == 0 ) {
		R.scan();
		Vector L = R-P;
		A = 1LL*K.y*L.z - 1LL*K.z*L.y;
		B = - 1LL*K.x*L.z + 1LL*K.z*L.x;
		C = 1LL*K.x*L.y - 1LL*K.y*L.x;
	}
	D = -( A*P.x + B*P.y + C*P.z );

	while( n ) {
		P.scan();
		if( A*P.x + B*P.y + C*P.z + D != 0 ) {
			puts("NIE");
			return 0;
		}
	}
	puts("TAK");
}

#include <iostream>

using namespace std;

int nwd(int a, int b) { return b ? nwd(b, a%b) : a; }
int magic_totient( int m ) {
	if( m % 2 == 0 ) m /= 2;
	if( m % 5 == 0 ) m /= 5, m *= 4;
	return m;
}

int pow( int a, int n, int m ) {
	int v = 1, _n = n, _a = a;
	if( n < 0 ) n = n%m + m;
	while( n ) {
		if( n % 2 == 1 )
			v = 1LL * v * a % m;
		a = 1LL * a * a % m;
		n /= 2;
	}
	cerr << "pow( " << _a << ", " << _n << ", " << m << " ) = " << v << endl;
	return v;
}

int pow_step( const int* c, int m ) {
	cerr << "pow_step( " << *c << ", " << m << ") " << endl;
	if( m == 1 ) return 0;
	if( *c == 1 ) return 1;
	//magic_totient( m )
	if( *c % 2 == 0 and m % 2 == 0 ) {
		int v = 1;
		while( m % 2 == 0 ) { v *= 2; m /= 2; }
		return 1LL * pow_step( c, m ) * pow( v, 64000000-1, m ) % m * v;
	}
	if( *c % 5 == 0 and m % 5 == 0 ) {
		int v = 1;
		while( m % 5 == 0 ) { v *= 5; m /= 5; }
		cerr << ":: " << v << endl;
		return 1LL * pow_step( c, m ) * pow( v, 64000000-1, m ) % m * v;
	}
	
	return pow( *c, pow_step( c+1, magic_totient(m) ), m );
}

void test() {
	int data[24];
	int n, m;
	cin >> n >> data[0];
	m = min(n,22);
	for( int i = 1; i <= m; i++ )
		cin >> data[i];
	
	data[m+1] = 1;
	
	cout << pow_step(data, 1000000) << '\n';
	
	for( m -= n; m --> 0; ) {
		int a;
		cin >> a;
	}
}
	
int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while( T --> 0 )
		test();
}
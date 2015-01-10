#include <iostream>
#include <vector>

using namespace std;

int nwd(int a, int b) {
	return b == 0 ? a : nwd(b, a%b);
}

int totient(int m) {
	int ret = m;
	for( int i = 2; i*i <= m; i++ )
		if( m % i == 0 )
			ret = 1LL*ret*(i-1)/i;
	return ret;
}

int pow( int a, int n, int m ) {
	int ret = 1;
	while( n ) {
		if( n&1 )
			ret = 1LL*ret*a%m;
		a = 1LL*a*a%m;
		n /= 2;
	}
	return ret;
}

void test() {
	int m, k, mod = 1000000;
	cin >> m >> k;
	
	vector<int> base;
	vector<int> modulo;
	base.reserve(m+1);
	modulo.reserve(m+1);
	
	base.push_back(k);
	
	while( m --> 0 ) {
		modulo.push_back( mod );
		mod = totient( mod / nwd(mod, k) );
		cin >> k;
		base.push_back(k);
	}
	
	cerr << ":: " << totient( mod / nwd(mod, k) ) << endl;
	
	while( base.size() > 1 ) {
		int n = base.back(); base.pop_back();
		int a = base.back(); base.pop_back();
		int m = modulo.back(); modulo.pop_back();
		
		cerr << ":: " << a << " " << m << endl;
		a = pow( a, n, m );
		base.push_back( a );
	}
	
	cout << base.back() << '\n';
}

int main() {
	ios_base::sync_with_stdio(false);
	
	int T;
	cin >> T;
	
	while( T --> 0 )
		test();
}

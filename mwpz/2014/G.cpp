#include <iostream>
#include <algorithm>
#include <limits>
#include <string>

using namespace std;

const int MOD = 1000000;

void mul( int& a, int b ) {
	long long v = 1LL*a*b;
	if( v >= 2*MOD )
		v = v%MOD+MOD;
	a = v;
}

int pow( int a, int n ) {
	int v = 1;
	while( n > 0 ) {
		if( n % 2 == 1)
			mul( v, a );
		mul( a, a );
		n /= 2;
	}
	return v;
}

void test() {
	int m, data[24];
	cin >> m;
	m = min(m,23);
	for( int i = 0; i <= m; i++ )
		cin >> data[i];
	
	int value = 1;
	for( int i = m; i >= 0; i-- )
		value = pow( data[i], value );
	
	cout << value%1000000 << '\n';
	
	cin.ignore(numeric_limits<streamsize>::max(),'\n');

}

int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while( T --> 0 )
		test();
}
#include <iostream>

using namespace std;

void test() {
	int m, n=2;
	cin >> m;
	
	if( m < 3 ) {
		cout << 1 << '\n';
		return;
	}
	
	for( int i = 3; i*i <= m; i += 2 )
		if( m % i == 0 ) {
			n *= 2;
			m /= i;
		}
		
	cout << n << '\n';
}
	
int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while( T --> 0 )
		test();
}
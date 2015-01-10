#include <iostream>

using namespace std;

void test() {
	int a, n;
	cin >> a >> n;
	
	a %= 10;
	int ret = 1;
	while( n ) {
		if( n&1 )
			ret = ret*a%10;
		a = a*a%10;
		n /= 2;
	}
	
	cout << ret << '\n';
}

int main() {
	ios_base::sync_with_stdio(false);
	
	int T;
	cin >> T;
	
	while( T --> 0 )
		test();
}

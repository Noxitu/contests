#include <iostream>

using namespace std;

void test() {
  int a, b;
  cin >> a >> b;
  int c = 0;
  while( a > 0 ) {
    int d = b;
	while( d > 0 ) {
	  c++;
	  d--;
	}
	a--;
  }
  cout << c << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
 
  while( T --> 0 )
    test();
}

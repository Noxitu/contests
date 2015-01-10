#include <iostream>
#include <vector>

using namespace std;
 
struct InputReader {
	int count;
	vector<int> data;
	
	InputReader() {
		data.resize(1);
		cin >> count;
		cin >> data[0];
	}
	
	~InputReader() {
		int v;
		while( count --> 0 )
			cin >> v;
	}
	
	int operator[](const int i) {
		
		while( i >= data.size() )
			if( data.size() <= count ) {
				int v;
				cin >> v;
				data.push_back(v);
				count--;
			} else {
				data.push_back(1);
			}
		
		return data[i];
	}
};

int licz( int i, int p2, int p5, int totalw ) {

}

void test() {
	InputReader input;
	licz( 0, 6, 6 );
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;
	
	cin >> T;
	while( T --> 0 )
		test();
}
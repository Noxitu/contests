#include <bits/stdc++.h>
using namespace std;

template<typename T>
inline ostream& operator<< (ostream& out, const vector<T>& data) {
	out << data[0];
	for( auto it = data.begin()+1; it != data.end(); it++ )
		out << ' ' << *it;
	return out;
}

template<typename T>
inline istream& operator>> (istream& in, vector<T>& data) {
	for( auto &v : data )
		in >> v;
	return in;
}


void test() {
	int n, sum = 0, min_odd = numeric_limits<int>::max();
    cin >> n;
    
    while( n --> 0 ) {
        int v;
        cin >> v;
        sum += v;
        if( v%2 == 1 and v < min_odd )
            min_odd = v;
    }
    
    if( sum%2 == 1 )
        sum -= min_odd;
    
    if( sum == 0 )
        cout << "NIESTETY\n";
    else
        cout << sum << '\n';
}

int main() {
	ios_base::sync_with_stdio(false);
    cin.tie(NULL);
	int T = 1;
	//cin >> T;
	while( T --> 0 )
		test();
	return 0;
}
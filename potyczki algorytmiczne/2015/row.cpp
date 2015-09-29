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

typedef unsigned long long uint64;

int score(uint64 v) {
    if( v == 0 )
        return 0;
    return score(v/10) + (v%10)*(v%10);
}

void test() {
	uint64 k, a, b;
    cin >> k >> a >> b;
    
    int answer = 0;
    for( int i = 0; i <= 18*81; i++ )
        if( k*i >= a and numeric_limits<uint64>::max()/k > i and k*i <= b and score(k*i) == i )
            answer++;
    
    cout << answer << '\n';
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
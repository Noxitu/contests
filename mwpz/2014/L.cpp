#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

typedef pair<int, int> point;
typedef pair< point, point > two_points;
typedef pair< int, two_points > line;

bool by_y(const point& a, const point& b) {
	return a.second < b.second;
}

inline int square(int a) {
	return a*a;
}

line make_line(const point& a, const point& b) {
	return line( square(a.first-b.first) + square(a.second-b.second), two_points(a,b) );
}

template<typename A, typename B>
ostream& operator<<(ostream& out, const pair<A,B>& p) {
	return out << p.first << " " << p.second;
}

void fix(int& c) {
	if( c > 4000 )
		c -= 4000;
}

template<typename A, typename B>
void fix(pair<A,B>& p) {
	fix(p.first);
	fix(p.second);
}


line closest_points( vector<point>::iterator begin, vector<point>::iterator end ) {
	if( end - begin < 2 )
		return line( 1000000000, two_points() );

	vector<point>::iterator mid = begin + (end-begin)/2;
	int split_at = mid->first;
	line best = min( closest_points( begin, mid ), closest_points( mid, end ) );
	
	inplace_merge( begin, mid, end, by_y );
	
	vector<point> middle;
	middle.reserve( end-begin );
	
	for( vector<point>::iterator it = begin; it != end; it++ )
		if( square(it->first - split_at) <= best.first )
			middle.push_back(*it);
			
	for( vector<point>::iterator first = middle.begin(); first != middle.end(); first++ )
		for( vector<point>::iterator second = first+1; second != middle.end() and square(second->second - first->second) <= best.first; second++ )
			best = min( best, make_line( *first, *second ) );
			
			
	return best;
}

void test() {
	int m;
	cin >> m;
	vector<point> data(4*m);
	for( int i = 0; i < m; i++ ) {
		cin >> data[4*i].first >> data[4*i].second;
		data[4*i+3] = data[4*i+2] = data[4*i+1] = data[4*i];
		data[4*i+3].first += 4000;
		data[4*i+3].second += 4000;
		data[4*i+2].first += 4000;
		data[4*i+1].second += 4000;
	}
		
	sort( data.begin(), data.end() );
	
	line answer = closest_points( data.begin(), data.end() );
	
	fix( answer.second );
	cout << answer.second << '\n';
}
	
int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while( T --> 0 )
		test();
}
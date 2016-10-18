#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
	int n;
	cin >> n;
	vector<pair<int, int> > tasks(n);
	while( n --> 0 ) {
		int a, b;
		cin >> a >> b;
		tasks[n].first = b;
		tasks[n].second = a;
	}
	sort( tasks.begin(), tasks.end() );
	int end = 1000000000;
	for( vector<pair<int, int> >::reverse_iterator it = tasks.rbegin(); it != tasks.rend(); it++ ) {
		end = min(it->first-it->second, end-it->second);	
	}
	cout << end << endl;
	return 0;
}
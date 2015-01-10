#include <cstdio>
//#include <message.h>
#include <algorithm>

void swap(int &a, int& b) { int t = a; a = b; b = t; }
template<typename _> inline _ min(const _ a, const _ b, const _ c) { return a < b ? (a < c ? a : c) : (b < c ? b : c); }

typedef std::pair<int,int> _;
char A[100002], B[100002];
_ T[100002];

int main() {
	//if( MyNodeId() != 0 )
	//	return 0;

	int n, m;
	scanf("%d %d", &n, &m );
	scanf("%s %s", A+1, B+1 );
	for( int x = 0; x <= n; x++ ) 
		T[x] = _(x,0);
	for( int y = 1; y <= m; y++ ) {
		_ previous = _(y,0);		
		for( int x = 1; x <= n; x++ ) {
			register _ addToSecond = _(T[x].first+1, T[x].second);
			register _ addToFirst = _(previous.first+1, previous.second);
			register _ swapLetters = _(T[x-1].first + (A[x] == B[y] ? 0 : 1), T[x-1].second + (A[x] < B[y] ? 1 : 0) );
			T[x-1] = previous;
			previous = min(addToSecond, addToFirst, swapLetters);
		}
		T[n] = previous;
			
	}
	printf("%d %d\n", T[n].first, T[n].second );
}

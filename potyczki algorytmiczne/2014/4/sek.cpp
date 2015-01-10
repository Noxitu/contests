#include <cstdio>
#include "message.h"
#include <algorithm>

void swap(int &a, int& b) { int t = a; a = b; b = t; }
template<typename _> inline _ min(const _ a, const _ b, const _ c) { return a < b ? (a < c ? a : c) : (b < c ? b : c); }

typedef std::pair<int,int> _;
char A[100002], B[100002];
_ T[100002];

int main() {
	int n, m;
	scanf("%d %d", &n, &m );
	scanf("%s %s", A+1, B+1 );


	int I = MyNodeId(), M = NumberOfNodes();
	int X1 = I*n/M, X2 = (I+1)*n/M;
	int PacketLength = m/900+5;


	for( int x = X1; x <= X2; x++ ) 
		T[x] = _(x,0);

	int PacketSize = 0, PacketSize2 = 0;
	for( int y = 1; y <= m; y++ ) {
		_ previous;
		if( I == 0 )
			previous = _(y,0);
		else {
			if( PacketSize == 0 ) {
				Receive(I-1);
				PacketSize = PacketLength;
			}
			previous.first = GetInt(I-1);
			previous.second = GetInt(I-1);
			PacketSize--;
		}

		for( int x = X1+1; x <= X2; x++ ) {
			register _ addToSecond = _(T[x].first+1, T[x].second);
			register _ addToFirst = _(previous.first+1, previous.second);
			register _ swapLetters = _(T[x-1].first + (A[x] == B[y] ? 0 : 1), T[x-1].second + (A[x] < B[y] ? 1 : 0) );
			T[x-1] = previous;
			previous = min(addToSecond, addToFirst, swapLetters);
		}
		T[X2] = previous;

		if( I+1 != M ) {
			PutInt(I+1, previous.first);
			PutInt(I+1, previous.second);
			PacketSize2++;
			if( PacketSize2 == PacketLength ) {
				Send(I+1);
				PacketSize2 = 0;
			}
		}	
	}

	if( I+1 == M ) {
		printf("%d %d\n", T[n].first, T[n].second );
	} else if( PacketSize2 > 0 )
		Send(I+1);

	return 0;
}

#include <iostream>

using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	int n, k;
	cin >> n >> k;
	
	int W[n][n][n];
	
	for( int i = 0; i < n; n++ )
		W[i][n-i][n] = 0;
	
	for( int i = 0; i < n; n++ )
		W[i][n-i-1][n] = 1;
		
	for( int M = n-2; M >= 0; M-- )
		for( int L = 0; L <= M; L++ ) {
			int R = M-L;
			W[L][R][O] = W[L+1][R][O] + W[L][R+1][O];
			for( int i = 0; i < O; i++ )
				W[L][R][O] += W[i][O-i][L+R+1];
		}
}
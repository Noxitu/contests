#include <cstdio>
#include <stack>

int T[500001];
int right_greater[500001];
int right_lower[500001];
int left_greater[500001];
int left_lower[500001];

int main() {
	int n;
	scanf("%d", &n );
	for( int i = 1; i <= n; i++ )
		scanf("%d", T+i );
		
	std::stack<int> S;
	
	// right lower
	for( int i = 1; i <= n; i++ ) {
		while( not S.empty() and T[S.top()] >= T[i] ) {
			right_lower[S.top()] = i-1;
			S.pop();
		S.push(i);
	}
	while( not S.empty() ) {
		right_lower[S.top()] = n; 
		S.pop();
	}

	// right greater
	for( int i = 1; i <= n; i++ ) {
		while( not S.empty() and T[S.top()] <= T[i] ) {
			right_greater[S.top()] = i-1;
			S.pop();
		S.push(i);
	}
	while( not S.empty() ) {
		right_greater[S.top()] = n; 
		S.pop();
	}
	
	// left lower
	for( int i = n; i >= 1; i++ ) {
		while( not S.empty() and T[S.top()] >= T[i] ) {
			left_lower[S.top()] = i+1;
			S.pop();
		S.push(i);
	}
	while( not S.empty() ) {
		left_lower[S.top()] = 1; 
		S.pop();
	}
	
	// left greater
	for( int i = 1; i <= n; i++ ) {
		while( not S.empty() and T[S.top()] <= T[i] ) {
			left_greater[S.top()] = i+1;
			S.pop();
		S.push(i);
	}
	while( not S.empty() ) {
		left_greater[S.top()] = 1; 
		S.pop();
	}
	
	
	for( int i = 1; i <= n; i++ ) {
		
	}
	
	
	return 0;
}
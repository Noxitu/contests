#include <cstdio>
#include <algorithm>
#include <stack>

int w,h;
int n,m; 
struct point_t {
	int x, y;
	int value;
	void scan() {
		scanf("%d %d %d", &x, &y, &value);
	}
	bool operator<(const point_t &o) const {
		return 1LL*h*(o.x-x) > 1LL*w*(y-o.y);
	}
	point_t operator-() const {
		point_t x = *this;
		x.value = -x.value;
		return x;
	}
};

point_t all[400000];
point_t guards[200000];
long long sumtree[200000];

bool compByY(const point_t &a, const point_t &b) {
	long long A = 1LL*h*(b.x-a.x), B = 1LL*w*(b.y-a.y);
	if( A == B )
		return b < a;
	return A > B;
}

void sumtree_set(point_t x, int value) {
	int l = 0, r = m;
	while( l < r ) {
		int mid = (l+r)/2;
		if( guards[mid] < x )
			l = mid+1;
		else {
			sumtree[mid] += value;
			r = mid;
		}
	}
}
long long sumtree_get(point_t x) {
	int l = 0, r = m;
	long long value = 0;
	while( l < r ) {
		int mid = (l+r)/2;
		if( x < guards[mid] )
			r = mid;
		else {
			value += sumtree[mid];
			l = mid+1;
		}
	}
	return value;
}

void append_stack(std::stack<std::pair<long long, point_t> >& stack, long long &profit) {
	while( not stack.empty() and stack.top().first < 0 )
		stack.pop();
	while( not stack.empty() ) {
		//printf("buying (%d, %d). cost = %d, profit = %lld\n", stack.top().second.x, stack.top().second.y, -stack.top().second.value, stack.top().first );
		profit += stack.top().first;
		stack.pop();
	}
	//puts("---");
}

int main() {
	scanf("%d %d %d %d", &n, &m, &w, &h );

	long long profit = 0;
	for( int i = 0; i < n; i++ ) {
		all[i].scan();
		profit += all[i].value;
	}

	for( int i = 0;	i < m; i++ ) {
		guards[i].scan();
		all[n+i] = -guards[i];
	}

	std::sort(guards, guards+m);

	for( int i = 0; i < n; i++ )
		sumtree_set(all[i], all[i].value);

	std::sort(all, all+n+m, compByY);

	profit -= sumtree_get(guards[m-1]);

	std::stack<std::pair<long long, point_t> > stack;
	for( int i = 0; i < n+m; i++ ) {
		if( all[i].value > 0 ) {
			sumtree_set(all[i], -all[i].value);
			continue;
		}
		long long value = sumtree_get(all[i]);

		if( stack.empty() ) {
			stack.push( std::make_pair( value+all[i].value, all[i] ) );
			continue;
		}
		if( all[i] < stack.top().second ) {
			if( value+all[i].value < 0 )
				stack.top().first -= value;
			else
				stack.top().first += all[i].value;
		} else {
			stack.top().first -= sumtree_get(stack.top().second);
			stack.push( std::make_pair( value+all[i].value, all[i] ) );
		}
		
		if( stack.top().first < 0 ) 
			append_stack( stack, profit );
	}
	append_stack( stack, profit );

	printf("%lld\n", profit );
}

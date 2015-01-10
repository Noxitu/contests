#include <cstdio>
#include <climits>
#include <cassert>
#include <cmath>

#define float long double
float elapsed;
float first_time;
float return_time;
float second_time;

long long nwd(long long a, long long b) {
	return b ? nwd(b,a%b) : a;
}

inline void step(const bool& up) {
	// Chapter I - Before first meeting
	if( elapsed < first_time ) {
		float left_end = elapsed + (up ? 2 : 1);
		float right_start = first_time - (up ? 0.5 : 1);
//		printf("First meeting ahead: %f -> %f and %f <- %f\n", elapsed, left_end, first_time, right_start );
		if( left_end < right_start ) {
			elapsed = left_end;
			first_time = right_start;
			return_time += up ? 1 : 2;
		} else {
			// y = a1 x 			x is part of edge (0..1)
			// y = a2 x + b2 		y is time (-elapsed)
			float a1 = up ? 2 : 1;
			float a2 = -(up ? 0.5 : 1);
			float b2 = first_time-elapsed;
			float part = b2/(a1-a2);
			elapsed += (up ? 2 : 1)*part;
			first_time = elapsed;
			return_time += (up ? 1 : 2)*part;
			return_time += first_time;
			second_time += return_time;
			elapsed += (up ? 1 : 0.5)*(1-part);
		}
		second_time -= up ? 1 : 2;
	} else
	// Chapter II - after first meeting
	if( elapsed < second_time ) {
		float right_end = elapsed + (up ? 1 : 0.5);
		float left_start = second_time - (up ? 1 : 2);
//		printf("Second meeting ahead: %f -> %f and %f <- %f\n", elapsed, right_end, second_time, left_start );
		if( right_end < left_start ) {
			elapsed = right_end;
			second_time = left_start;
		} else {
			float a1 = up ? 1 : 0.5;
			float a2 = -(up ? 1 : 2);
			float b2 = second_time-elapsed;
			float part = b2/(a1-a2);
			second_time = elapsed + (up ? 1 : 0.5)*part;
			elapsed = second_time;
		}
	}
}

void test() {
	int n;
	scanf("%d", &n);
	n *= 2;
	elapsed = return_time = 0;
	first_time = 0.75*n;
	second_time = 1.5*n;

	for( int i = n; i; ) {
		static char buf[2];
		int v;
		scanf("%1s", buf);
		sscanf(buf, "%x", &v);
		for( int j = 4; j-- and i--; )
			step(v & (1<<j));
	}
//	printf("First meeting: %f\n", first_time );
//	printf("Return time: %f\n", return_time );
//	printf("Second time: %f\n", second_time );
	long long q = 25*16*9, p = q*second_time+0.5;
	long long x= nwd(p, q);
	printf("%lld/%lld\n", p/x, q/x );
}

int main() {
	int T;
	scanf("%d", &T );
	while( T --> 0 )
		test();
}

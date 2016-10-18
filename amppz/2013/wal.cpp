#include <cstdio>

char T[100100];

int main() {
	int n, k;
	double p;
	scanf("%d %d %lf", &n, &k, &p );
	printf("%.12lf\n", p / (p+(1-p)*(n-k)/n) );
}

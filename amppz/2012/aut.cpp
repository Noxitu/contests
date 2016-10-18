#include <cstdio>

inline int min(int a, int b) { return a < b ? a : b; }

int zysk[41][41][40*40+1], L[41], C[41];

int main() {
    int n, k;
    scanf("%d %d", &n, &k );
    if( k > 40*40 )
        k = 40*40;
    for( int i = 1; i <= n; i++ )
        scanf( "%d", C+i );

    for( int i = 1; i <= n; i++ )
        scanf( "%d", L+i );

    for( int i = 1; i <= n; i++ ) {
        for( int l = 0; l <= 40; l++ ) {
            for( int s = 0; s <= k; s++ ) {
                int best = 0;
                for( int j = 0; j <= L[i]; j++ ) {
                    if( l+j > 40 or s < j*C[i] )
                        break;
                    int val = zysk[i-1][l+j][s-j*C[i]] + min( l+j, L[i] )*C[i];
                    if( val > best )
                        best = val;
                }
                zysk[i][l][s] = best;
            }
        }
    }
    printf("%d\n", zysk[n][0][k] );
}

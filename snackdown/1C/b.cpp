#include <iostream>
#include <algorithm>
 
using namespace std;

typedef pair< int, pair< pair< int, int >, pair<int, int> > > wynik;
 
void test() {
    int n;
    cin >> n;
    int T[n+1];
    for( int i = 1; i <= n; i++ )
        T[i] = 0;
        
    for( int i = 1; i <= 2*n-4; i++ ) {
        int v;
        cin >> v;
        T[v]++;
    }
    
    int ai = 1;
    for( int i = 1; i <= n-2; i++ )
        if( T[i] > T[ai] )
            ai = i;
            
    int bi = ai == 1 ? 2 : 1;
    for( int i = 1; i <= n-2; i++ )
        if( i != ai and T[i] > T[bi] )
            bi = i;
            
    int m = n > 3 ? 4 : 3;
    int V[] = { n-1, n, ai, bi };
    wynik w = make_pair( -1, make_pair( make_pair( 0, 0 ), make_pair( 0, 0 ) ) );
    for( int a = 0; a < m; a++ )
        for( int b = a+1; b < m; b++ ) 
            for( int c = 0; c < m; c++ )
                for( int d = c+1; d < m; d++ )
                    if( a != b and c != d and a != 0 and b != 0 and c != 1 and d != 1 ) {
                        int Z[] = {a,b,c,d};
                        
                        for( int i = 0; i < 4; i++ ) {
                            T[ V[ Z[ i ] ] ]++;
                        }
                        int score = 2;
                        int min_v = *min_element( T+1, T+n+1 );
                        int max_v = *max_element( T+1, T+n+1 );
                        for( int i = n-1; i <= n; i++ )
                            if( T[i] == min_v or T[i] == max_v )
                                score--;
                               
                        wynik k = make_pair( score, make_pair( make_pair( V[a], V[b] ), make_pair( V[c], V[d] ) ) );
                        w = max( w, k );
                        for( int i = 0; i < 4; i++ ) {
                            //i[Z][V][T]--;
                            T[ V[ Z[ i ] ] ]--;
                        }
                    }
    const char* ans[] = {"none", "one", "both"};
    cout << ans[w.first] << '\n' << w.second.first.first << ' ' << w.second.first.second << '\n' << w.second.second.first << ' ' << w.second.second.second << '\n';
}

 
int main() {
    ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    while( T --> 0 )
        test();

}
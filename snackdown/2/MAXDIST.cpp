#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

int V[10001];
vector<int> E[10001];
int N;

int bfs(int a) {
    for( int i = 1; i <= N; i++ )
        V[i] = -1;
        
    queue<int> q;
    V[a] = 0;
    q.push(a);
    int v;
    while( not q.empty() ) {
        v = q.front(); q.pop();
        for( auto& u : E[v] )
            if( V[u] == -1 ) {
                V[u] = V[v] + 1;
                q.push(u);
            }
    }
    return v;
}

void test() {
    cin >> N;
    for( int i = 1; i <= N; i++ )
        E[i].clear();
    
    for( int i = 1; i < N; i++ ) {
        int a, b;
        cin >> a >> b;
        E[a].push_back(b);
        E[b].push_back(a);
    }
    
    if( N == 2 ) {
        cout << "NO\n";
        return;
    }
    
    int a = bfs(1);
    int b = bfs(a);
    int L = V[b], nA = 0;
    for( int i = 1; i <= N; i++ )
        if( V[i] == L )
            nA++;
         
    int c = bfs(b);
    int nB = 0;
    for( int i = 1; i <= N; i++ )
        if( V[i] == L )
            nB++;
    cout << ( nL > 1 and nR > 1 ? "NO\n" : "YES\n");
}

int main() {
    int T;
    cin >> T;
    while( T --> 0 )
        test();
}
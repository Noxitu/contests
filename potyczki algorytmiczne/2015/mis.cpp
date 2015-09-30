#include <bits/stdc++.h>
using namespace std;

template<typename T>
inline ostream& operator<< (ostream& out, const vector<T>& data) {
    out << data[0];
    for( auto it = data.begin()+1; it != data.end(); it++ )
        out << ' ' << *it;
    return out;
}

template<typename T>
inline istream& operator>> (istream& in, vector<T>& data) {
    for( auto &v : data )
        in >> v;
    return in;
}

struct vertex_t : public vector<vertex_t*> {
    int count;
    int group;
};


void test() {
    int n, m, d;
    cin >> n >> m >> d;
    vertex_t V[n+1];
    
    while( m --> 0 ) {
        int a, b;
        cin >> a >> b;
        V[a].push_back(V+b);
        V[b].push_back(V+a);
    }
    
    queue<vertex_t*> Q;
    
    for( int i = 1; i <= n; i++ ) {
        V[i].group = 0;
        V[i].count = V[i].size();
        if( V[i].count < d ) {
            Q.push(V+i);
        }
    }
    
    while( not Q.empty() ) {
        vertex_t *v = Q.front();
        Q.pop();
        
        for( vertex_t *u : *v ) {
            if( u->count == d )
                Q.push(u);
            u->count--;
        }
    }
    
    pair<int, int> best = make_pair(0,0);
    
    for( int i = 1; i <= n; i++ ) {
        int size = 0;
        
        if( V[i].group == 0 and V[i].count >= d ) 
            Q.push(V+i);
        
        while( not Q.empty() ) {
            vertex_t *v = Q.front();
            Q.pop();
            
            v->group = i;
            size++;
            
            for( vertex_t *u : *v ) {
                if( u->count >= d and u->group == 0 ) {
                    u->group = i;
                    Q.push(u);
                }
            }
        }
        
        best = max( best, make_pair(size, i) );
    }
    
    if( best.first == 0 ) {
        cout << "NIE\n";
        return;
    }
    
    cout << best.first << '\n';
    for( int i = 1; i <= n; i++ )
        if( V[i].group == best.second )
            cout << i << ' ';   
    cout << '\n';
    
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int T = 1;
    //cin >> T;
    while( T --> 0 )
        test();
    return 0;
}
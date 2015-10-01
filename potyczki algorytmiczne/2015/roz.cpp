#include <bits/stdc++.h>
#include <string>
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

template<typename A, typename B>
inline ostream& operator<< (ostream& out, const pair<A, B>& data) {
    return out << data.first << ' ' << data.second;
}

struct vertex_t : public vector<vertex_t*> {
    bool inside;
    int missing = 0;
    bool updated = false;
    pair<int, int> value = make_pair(0, numeric_limits<int>::max());
    
    void calculate_value() {
        vector<int> events;
        for( auto v : *this ) {
            events.push_back( v->value.first );
            events.push_back( v->value.second );
        }
        
        sort( events.begin(), events.end() );
        value.first = events[size()-1];
        value.second = events[size()];
    }
    
    long long update_neighbours() const {
        long long ret = 0;
        
        for( auto v : *this ) {
            if( v->value.second <= value.first )
                v->value.first = v->value.second;
            else if( value.second <= v->value.first )
                v->value.second = v->value.first;
            else
                v->value.first = v->value.second = value.first;
                
            ret += abs(value.first-v->value.first);
        }
        return ret;
    }
};

void test() {
    int n, m;
    cin >> n >> m;
    
    vertex_t V[n];
    
    for( int i = 0; i < n; i++ ) {
        V[i].inside = (i>=m);
    }
        
    for( int i = 0; i < n-1; i++ ) {
        int a, b;
        cin >> a >> b;
        a--; b--;
        V[a].push_back(V+b);
        V[b].push_back(V+a);
        
        if( V[a].inside )
            V[b].missing++;
            
        if( V[b].inside )
            V[a].missing++;
    }
    
    for( int i = 0; i < m; i++ ) {
        int a;
        cin >> a;
        V[i].value = make_pair(a, a);
    }
    
    queue<vertex_t*> Q;
    for( auto& v : V ) {
        if( v.inside and v.missing <= 1 )
            Q.push(&v);
    }
    
    vertex_t* last_vertex = V;
    while( not Q.empty() ) {
        vertex_t* v = Q.front();
        Q.pop();
        
        v->calculate_value();
        
        for( auto u : *v ) {
            u->missing--;
            if( u->missing == 1 )
                Q.push(u);
        }
        
        last_vertex = v;
    }
    
    Q.push(last_vertex);
    last_vertex->value.second = last_vertex->value.first;
    last_vertex->updated = true;
    
    long long result = 0;
    while( not Q.empty() ) {
        vertex_t* v = Q.front();
        Q.pop();
        
        result += v->update_neighbours();
        
        for( auto u : *v )
            if( not u->updated ) {
                Q.push(u);
                u->updated = true;
            }
    }
    
    cout << result/2 << '\n';
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
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

struct event_t {
    int id;
    long long d, b;
    event_t( int id, long long d=0, long long b=0 ) : id(id), d(d), b(b) {}
    
    friend bool operator< (const event_t& a, const event_t& b) {
        return a.id > b.id;
    }
};


void test() {
    int n, m;
    cin >> n >> m;
    vector<int> ids(n);
    for( int i = 0; i < n; i++ )
        ids[i] = i;
    
    vector<int> growth(n);
    cin >> growth;
    sort( growth.begin(), growth.end() );
    
    vector<long long> cumulated(n+1);
    cumulated[0] = 0;
    for( int i = 0; i < n; i++ )
        cumulated[i+1] = growth[i]+cumulated[i];
        
    deque<event_t> cuts;
    cuts.push_back( event_t( -1 ) );
        
    while( m --> 0 ) {
        long long d, b;
        cin >> d >> b;
        
        auto it = lower_bound( ids.begin(), ids.end(), -1/*ignored*/, [&cuts, &growth, &d, &b](int id,int){
            auto& last_cut = *lower_bound( cuts.begin(), cuts.end(), event_t(id) );
            long long size = (d-last_cut.d)*growth[id] + last_cut.b;
            return size < b;
        });
        
        if( it == ids.end() ) {
            cout << "0\n";
            continue;
        }
        
        int id = *it;
        
        long long score = 0;
        int top_id = n;
        
        auto update = [&top_id, &score, &cumulated, &d, &b](const event_t& e) {
            long long delta = cumulated[top_id] - cumulated[e.id];
            int how_many = top_id-e.id;
            long long time = d-e.d;
            long long height = b-e.b;
            score += delta*time - how_many*height;
            top_id = e.id;
        };
        
        while( cuts.front().id >= id ) {
            update( cuts.front() );
            cuts.pop_front();
        }
        
        update( event_t( id, cuts.front().d, cuts.front().b ) );
            
        cuts.push_front( event_t( id, d, b ) );
         
        cout << score << '\n';
    }
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
#include "../biblioteczka.hpp"

int main() { return main_one(); }

struct vertex_t;

struct island_t {
    vector<pair<island_t*, int>> edges;
    vector<pair<int, int>> traps;
    vector<vertex_t*> vertices;
    bool destination = false;
};

struct vertex_t {
    island_t* island;
    long long a, b;
    long long distance;
};

constexpr long long INF = numeric_limits<long long>::max()/2;

inline ostream& operator<< (ostream& out, const vertex_t* v) {
    return out << '(' << v->a << ' ' << v->b << ')';
}

void test() {
    int n, m;
    cin >> n >> m;
    
    vector_from_one<island_t> islands(n);
    while( m --> 0 ) {
        int a, b, d;
        cin >> a >> b >> d;
        islands[a].edges.push_back(make_pair(&islands[b], d));
    }
    islands.back().destination = true;
    
    int p;
    cin >> p;
    while( p --> 0 ) {
        int w;
        pair<int, int> when;
        cin >> w >> when;
        islands[w].traps.push_back(when);
    }
    
    for( auto& island : islands ) {
        sort( begin(island.traps), end(island.traps) );
        
        int safe_since = 1;
        for( auto t : island.traps ) {
            if( safe_since < t.first )
                island.vertices.push_back(new vertex_t{ &island, safe_since, t.first-1, -1 });
            safe_since = max(safe_since, t.second+1);
        }
        island.vertices.push_back(new vertex_t{ &island, safe_since, INF, -1 });
        
    }
    
    priority_queue<pair<long long, vertex_t*>> Q;
    Q.push( make_pair(-1, islands[1].vertices[0]) );
    
    while( not Q.empty() ) {
        long long dist = -Q.top().first;
        vertex_t* v = Q.top().second;
        //cerr << "Visiting " << (v->island-&islands[1]+1) << " at " << make_pair(dist, v->b) << endl;
        Q.pop();
        
        if( v->distance != -1 )
             continue;
             
        v->distance = dist;
        for( auto e : v->island->edges ) {
            auto last = end(e.first->vertices);
            auto it = lower_bound( begin(e.first->vertices), last, dist+e.second, [](vertex_t* v, long long x)->bool{ return v->b < x; } );
            //cerr << "LOL: " << e.first->vertices << " search for " << dist+e.second << " found ";
            //if( it != last ) cerr << *it << endl;
            //else cerr << "NONE" << endl;
            for( ; it != last and (*it)->a <= v->b+e.second; it++ ) 
                if( (*it)->distance == -1 ) {
                    //cerr << "  Can go to " << ((*it)->island-&islands[1]+1) << " at " << make_pair(dist+e.second, v->b+e.second) << " safe at " << make_pair( (*it)->a, (*it)->b ) << endl;
                    long long dist2 = max( (*it)->a, dist+e.second );
                    Q.push( make_pair(-dist2, *it) );
                }
        }
    }
    
    long long answer = islands.back().vertices.front()->distance;
    if( answer == -1 )
        cout << "NIE" << endl;
    else 
        cout << answer-1 << endl;
    
}
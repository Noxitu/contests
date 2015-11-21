#include "../biblioteczka.hpp"

int main() { return main_one(); }

struct Vertex {
    long long destroy_cost, split_cost;
    vector<Vertex*> down, up;
};


void test() {
    int n;
    cin >> n;
    vector_from_one<Vertex> heads(n);
    for( auto& h : heads ) {
        int count;
        cin >> h.split_cost >> h.destroy_cost >> count;
        while( count --> 0 ) {
            int g;
            cin >> g;
            h.down.push_back(&heads[g]);
            heads[g].up.push_back(&h);
        }
    }
    
    priority_queue<pair<long long, Vertex*>> Q;
    for( auto& h : heads ) {
        for( auto v : h.down )
            h.split_cost += v->destroy_cost;
            
        Q.push(make_pair(-h.destroy_cost, &h));
        Q.push(make_pair(-h.split_cost, &h));
    }
    
    while( not Q.empty() ) {
        long long cost = -Q.top().first;
        Vertex* v = Q.top().second;
        Q.pop();
        
        if( cost >= v->destroy_cost )
            continue;
            
        for( auto u : v->up ) {
            u->split_cost -= (v->destroy_cost - cost);
            Q.push(make_pair(-u->split_cost, u));
        }
            
        v->destroy_cost = cost;
    }
    
    cout << heads[1].destroy_cost << endl;
}
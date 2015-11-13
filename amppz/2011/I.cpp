#include <bits/stdc++.h>
using namespace std;

class Vertex;

class Edge {
    public:
        Vertex *target;
        Edge() {};
        Edge(Vertex *target) : target(target) {}
        int flow_left() const { return flow_limit()-flow(); }
        virtual int flow() const = 0;
        virtual int flow_limit() const = 0;
        virtual void add_flow(int change) = 0;
};

class NormalEdge : public Edge {
    private:
        int _flow;
    public:
        NormalEdge(Vertex *target) : Edge(target), _flow(0) {}
        int flow() const { return _flow; }
        int flow_limit() const { return numeric_limits<int>::max(); }
        void add_flow(int change) { _flow += change;}
};

class InvertedEdge : public Edge {
    private:
        Edge *edge;
    public:
        InvertedEdge(Vertex *target, Edge *edge) : Edge(target), edge(edge) {}
        int flow() const { return 0; }
        int flow_limit() const { return edge->flow(); }
        void add_flow(int change) { edge->add_flow(-change); }
};

class Vertex : public vector<Edge*> {
    public:
        int distance;
        Vertex::iterator iter;
};

class Person : public Vertex, public Edge {
    private:
        int used;
    public:
        bool in_answer;
        int iq;
        int flow() const { return used; }
        int flow_limit() const { return iq; }
        void add_flow(int change) { used += change; }
        
        Person() : used(0), in_answer(false) {}
};

class BipartiteGraph {
    public:
        int n, m;
        Vertex source, sink;
        Person *all, *U, *V;
        
        BipartiteGraph(int n, int m) : n(n), m(m) {
            all = new Person[n+m];
            U = all-1;
            V = U+n;
            
            for( int a = 1; a <= n; a++ ) {
                U[a].target = U+a;
                source.push_back(U+a);
            }
            
            for( int b = 1; b <= m; b++ ) {
                V[b].target = &sink;
                V[b].push_back(V+b);
            }
        }
        
        void add_edge(int a, int b) {
            Edge *e = new NormalEdge(V+b);
            U[a].push_back(e);
            V[b].push_back( new InvertedEdge(U+a, e) ); 
        }

        bool bfs() {
            source.distance = 0;
            sink.distance = -1;
            for( Person *p = all; p != all+n+m; p++ )
                p->distance = -1;
                
            queue<Vertex*> Q;
            Q.push(&source);
            while( not Q.empty() ) {
                Vertex *v = Q.front();
                Q.pop();
                for( Vertex::iterator it = v->begin(); it != v->end(); it++ ) 
                    if( (*it)->target->distance == -1 and (*it)->flow_left() > 0 ) {
                        (*it)->target->distance = v->distance+1;
                        Q.push((*it)->target);
                    }
            }
            return sink.distance != -1;
        }
        
        long long dfs() {
            source.iter = source.begin();
            sink.iter = sink.begin();
            for( Person *p = all; p != all+n+m; p++ )
                p->iter = p->begin();
                
            return dfs(&source, numeric_limits<long long>::max());
        }
        
        long long dfs(Vertex *v, long long max_flow ) {
            if( v == &sink ) {
                return max_flow;
            }
                
            long long added = 0;
            for( ; v->iter != v->end(); v->iter++ ) {
                Edge* e = *(v->iter);
                if( e->target->distance != v->distance+1 or e->flow_left() == 0 )
                    continue;
                    
                long long flow_found = dfs( e->target, min(max_flow, (long long) e->flow_left()) );
                e->add_flow(flow_found);
                added += flow_found;
                max_flow -= flow_found;
                
                if( max_flow == 0 )
                    break;
            }
            return added;
        }
        
        long long EdmondsKarp() {
            long long flow = 0;
            while( bfs() ) {
                flow += dfs();
            }
            return flow;
        }
    
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
	int n, m, k;
	cin >> n >> m >> k;
    
    bool edges[n+1][m+1];
    for( int a = 1; a <= n; a++ )
        for( int b = 1; b <= m; b++ )
            edges[a][b] = false;
            
	while( k --> 0 ) {
		int a, b;
		cin >> a >> b;
		edges[a][b] = true;
    }
    
    BipartiteGraph G(n,m);
    for( int a = 1; a <= n; a++ )
        for( int b = 1; b <= m; b++ )
            if( not edges[a][b] ) {
                G.add_edge(a,b);
            }
    
    for( int i = 1; i <= n; i++ )
        cin >> G.U[i].iq;
        
    for( int j = 1; j <= m; j++ )
        cin >> G.V[j].iq;
        
    G.EdmondsKarp();
      
    long long answer = 0;
    int u_n = 0, v_n = m;
    
    G.bfs();
    for( int i = 1; i <= n; i++ ) {
        if( G.U[i].distance != -1 ) {
            answer += G.U[i].iq;
            G.U[i].in_answer = true;
            u_n++;
        }
    }
    
    for( int j = 1; j <= m; j++ ) 
        G.V[j].in_answer = true;
        
    for( int i = 1; i <= n; i++ ) {
        if( G.U[i].in_answer )
            for( int j = 1; j <= m; j++ )
                if( G.V[j].in_answer and not edges[i][j] ) {
                    v_n--;
                    G.V[j].in_answer = false;
                }
                
    }
    
    for( int j = 1; j <= m; j++ )
        if( G.V[j].in_answer )
            answer += G.V[j].iq;
    
    cout << answer << '\n' << u_n << '\n';
    for( int i = 1; i <= n; i++ )
        if( G.U[i].in_answer )
            cout << i << ' ';
    cout << '\n' << v_n << '\n';
    for( int j = 1; j <= m; j++ )
        if( G.V[j].in_answer )
            cout << j << ' ';
    cout << '\n';
}

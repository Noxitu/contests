#include <iostream>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

char W[10001];
int T[10001];
map<int, int> answers;
set<pair<int, int> > to_remove;

struct question {
    int i;
    int a, b;
    int answer;
};

bool byB( question a, question b ){
    return a.b < b.b;
}

bool byI( question a, question b ){
    return a.i < b.i;
}

void prep() {
    map< pair<int, int>, int > M;
    int N = 0;
    
    for( ; W[N]; N++ )
        T[N] = W[N]-'a';
        
    for( int l = 1; l < N; l *= 2 ) {
        M.clear();
        for( int i = 0; i < N; i++ ) {
            auto p = make_pair( T[i], i+l<N ? T[i+l] : 0 );
            M[p] = 0;
        }
        
        {
            int i = 1;
            for( auto& it : M )
                it.second = i++;
        }
        
        for( int i = 0; i < N; i++ ) {
            auto p = make_pair( T[i], i+l<N ? T[i+l] : 0 );
            T[i] = M[p];
        }
    }
    
    for( int i = 0; i < N; i++ ) {
        if( answers.find(T[i]) == answers.end() )
            answers[T[i]] = i;
    }
    
    for( int i = 0; i < N; i++ ) {
        to_remove.insert( make_pair(i+1, N-i-1) );
    }
}

void test() {

}

int main() {
    int M;
    cin >> W >> M;
    prep();
    question Q[M];
    for( int i = 0; i < M; i++ ) {
        Q[i].i = i;
        Q[i].answer = -1;
        cin >> Q[i].a >> Q[i].b;
    }
    sort( Q, Q+M, byB );
    
    int l = 1;
    for( auto& q : Q ) {
        while( not to_remove.empty() and (*to_remove.begin()).first < q.b ) {
            int pos = (*to_remove.begin()).second;
            answers.erase(T[pos]);
            to_remove.erase(to_remove.begin());           
        }
        for( auto i : answers ) 
            cout << W+i.second << ' ' << i.second << endl;
        cout << endl;
        
        auto it = answers.find(T[q.a-1]);
        it++;
        //cout << '#' << (W+it->second) << endl;
        if( it != answers.end() )
            q.answer = it->second;
            
    }
    
    sort( Q, Q+M, byI );
    for( auto q : Q )
        cout << q.answer << '\n';
}
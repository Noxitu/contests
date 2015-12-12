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
inline ostream& operator<< (ostream& out, const queue<T>& data) {
    switch( data.size() ) {
        case 0: return out << "[]";
        case 1: return out << "[ " << data.front() << " ]";
        default: return out << "[ " << data.front() << " ... ]";
    }
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

int gcd(int a, int b) { return b==0 ? a : gcd(b, a%b); }

long long test( vector<int>& players, vector<char>& moves ) {
    int step = players.size() % moves.size();
    int high = 0;
    int sum = 0;
    int low = 0;
    
    auto update_score = [](int &score, char move) {
          score += (move == 'W' ? 1 : -1);
    };
    
    for( int i = 0, m = 0; i < moves.size(); i++ ) {
        update_score( sum, moves[m] );
        high = max( high, sum );
        low = min( low, sum );
        m = (m+step) % moves.size();
    }
    
    low = min( low, sum+low );
    high = max( high, sum+high );
    
    queue<int> levels[high-low+1];
    
    int min_score = 0;
    auto level_push = [&min_score, &levels, low](int score, int pos) {
        levels[score-low].push(pos);
        if( min_score > score )
            min_score = score;
    };
    auto level_pop = [&min_score, &levels, low](int score) {
        levels[score-low].pop();
        while( levels[min_score-low].empty() )
            min_score++;
    };
    auto level_get = [&levels, low](int score)->int {
        return levels[score-low].front();
    };
    
    
    int score_end = 0;
    level_push( score_end, -1 );
    for( int i = 0, m = 0; i < moves.size(); i++ ) {
        update_score( score_end, moves[m] );
        level_push( score_end, i );
        m = (m+step) % moves.size();
    }
    
    long long answer = numeric_limits<long long>::max();
    int score_begin = 0;
    for( int i = 0, m = 0; i < moves.size(); i++ ) {
        for( int p = m; p < players.size(); p += moves.size() ) {
            int from_loops = max( 0, players[p] - (score_begin-min_score) );
            
            if( from_loops > 0 and sum >= 0 )
                continue;
                
            //from_loops -= from_loops % -sum;
            //int loops = from_loops / -sum;
            int loops = from_loops == 0 ? 0 : (from_loops-sum-1)/-sum;
            int score_diff = players[p] + loops*sum;
            int x = level_get(score_begin-score_diff) - i;
            
            long long loses = p + 1 + 1LL*loops*players.size()*moves.size() + 1LL*players.size()*x;
            answer = min( answer, loses );
        }
        
        level_pop( score_begin );
        update_score( score_begin, moves[m] );
        update_score( score_end, moves[m] );
        level_push( score_end, i+moves.size() );
        m = (m+step) % moves.size();
    }
    

    return answer;
}

void test() {
    int n;
    cin >> n;
    vector<int> players(n);
    cin >> players;
    
    int m;
    cin >> m;
    vector<char> moves(m);
    cin >> moves;
    
    int d = gcd(n, m);
    
    vector<int> players_skip(n/d);
    vector<char> moves_skip(m/d);
    
    long long answer = numeric_limits<long long>::max();
    for( int i = 0; i < d; i++ ) {
        for( int j = 0; j < n/d; j++ )
            players_skip[j] = players[d*j+i];
            
        for( int j = 0; j < m/d; j++ )
            moves_skip[j] = moves[d*j+i];
            
        long long result = test(players_skip, moves_skip);
        if( result != numeric_limits<long long>::max() )
            answer = min( answer, result*d+i-d+1 );
    }
   
    if( answer == numeric_limits<long long>::max() )
        cout << "-1\n";
    else
        cout << answer << '\n';
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
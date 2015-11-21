#include "../biblioteczka.hpp"

int main() { return main_one(); }

void test() {
    int n, k;
    cin >> n >> k;
    
    vector<int> genes(n);
    cin >> genes;
    
    reverse( begin(genes), end(genes) );
    
    vector_from_one<vector<int>> positions(1000000);
    for( int i = 0; i < n; i++ )
        positions[genes[i]].push_back(i);
        
    vector<int> next(n);
    for( int i = 1; i <= positions.size(); i++ ) {
        const auto &pos = positions[i];
        for( auto it = begin(pos); it != end(pos); it++ )
            if( it+k-1 < end(pos) )
                next[*it] = *(it+k-1);
            else
                next[*it] = -1;
    }
    
    vector<int> score(n, 0);
    vector<int> best_score(n, 0);
    for( int i = 0; i < n; i++ ) {
        if( i != 0 )
            best_score[i] = max(score[i], best_score[i-1]);
            
        int j = next[i];
        if( j != -1 )
            score[j] = max(score[j], best_score[i-1]+1);
    }
    
    reverse( begin(genes), end(genes) );
    reverse( begin(score), end(score) );
    reverse( begin(best_score), end(best_score) );
    
    cout << best_score.front()*k << '\n';
    
    for( int current_score = best_score.front(), first = 0, second = 0; current_score > 0 ; current_score-- ) {
        int lowest = numeric_limits<int>::max();
        
        for( ; best_score[second] == current_score; second++ )
            if( score[second] == current_score )
                lowest = min(lowest, genes[second] );
                
        int m = k;
        for( ; m > 0; first++ ) {
            if( genes[first] == lowest ) {
                m--;
                cout << lowest << ' ';
            }
        }
        
        second = first;
    }
    
    cout << endl;
}
#include "../biblioteczka.hpp"

int main() { return main_many(); }

void test()
{
    int n;
    cin >> n;
    
    string weather;
    cin >> weather;
        
    vector<int> best(n+1, 0);
        
    int finalized_score = 0;
    for( int start = 0; start <= n; ++start )
    {
        int sunny = 0;
        int days = 0;
        for( int i = start; i < n; ++i )
        {
            sunny += weather[i] == '1' ? 1 : -1;
            ++days;
            
            if( sunny > 0 )
                best[i+1] = max(best[i+1], finalized_score + days);
        }
        
        finalized_score = max(finalized_score, best[start]);
    }
    
    cout << finalized_score << '\n';
}

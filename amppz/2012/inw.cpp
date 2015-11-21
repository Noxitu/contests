#include "../biblioteczka.hpp"

int main() { return main_one(); }

void test() {
    int n;
    cin >> n;
    
    vector_from_one<int> values(n);
    cin >> values;
    
    vector<int> answer;
    for( int i = 1, highest = 0; i <= n; i++ ) {
        highest = max(highest, values[i]);
        if( highest == i )
            answer.push_back(i);
    }
    
    cout << answer.size() << '\n';
    int i = 1;
    for( auto highest : answer ) {
        cout << highest-i+1;
        for( ; i <= highest; i++ )
            cout << ' ' << i;
        cout << '\n';
    }
}
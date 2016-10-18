#include "../biblioteczka.hpp"

int main() { return main_many(); }

void test()
{
    unsigned int N, n, m;
    cin >> N >> n >> m;
    
    std::string input;
    cin >> input;
    
    if( input.size() != n )
    {
        cout << "Świetne testy bulwo!\n";
        return;
    }
    
    std::vector<int> blocks;
    {
        int current_block = N-n;
        for(char c : input)
        {
            ++current_block;
            if(c == '1')
            {
                blocks.push_back(current_block);
                current_block = 0;
            }   
        }
    }
    
    int with_0 = 1, with_1 = 1;
    
    for(int size : reversed(blocks))
    {
        with_0 = (with_0 + (size-1) * with_1) % m;
        with_1 = (with_0 + with_1) % m;
    }
    
    cout << with_0 << '\n';
}

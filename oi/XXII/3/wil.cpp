#include <bits/stdc++.h>


// std::vector 
template<typename T>
inline std::ostream& operator<< (std::ostream& out, const std::vector<T>& data) {
    out << data[0];
    for( typename std::vector<T>::const_iterator it = data.begin()+1; it != data.end(); it++ )
        out << ' ' << *it;
    return out;
}

template<typename T>
inline std::istream& operator>> (std::istream& in, std::vector<T>& data) {
    for( typename std::vector<T>::iterator it = data.begin(); it != data.end(); it++ )
        in >> *it;
    return in;
}

void test();

int main_one() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    test();
    return 0;
}

using namespace std;

// ================================================================

int main() { return main_one(); }

void fill(long long &available, multiset<int> &used, multiset<int> &unused)
{
    while( available < 0 )
    {
        multiset<int>::iterator last = used.end();
        --last;
        unused.insert(*last);
        available += *last;
        used.erase(last);    
    }
    
    while( not unused.empty() and available >= *unused.begin() )
    {
        multiset<int>::iterator first = unused.begin();
        used.insert(*first);
        available -= *first;
        unused.erase(first);   
    }
}

void test()
{
    int n, d;
    long long p;
    
    cin >> n >> p >> d;
    
    vector<int> holes(n);
    cin >> holes;
    
    d = min(n, d);
    
    multiset<int> used, unused( holes.begin()+d, holes.end() );
    
    fill(p, used, unused);
    size_t best = used.size();
    
    for( vector<int>::iterator it = holes.begin(); it+d != holes.end(); ++it )
    {
        {
            int value = *(it+d);
            multiset<int>::iterator removed = unused.find(value);
            if(removed != unused.end())
            {
                unused.erase(removed);
            }
            else
            {
                removed = used.find(value);
                used.erase(removed);
                p += value;
            }
        }
        
        {
            int value = *it;
            used.insert(value);
            p -= value;
            
            fill(p, used, unused);
        }
        
        for(auto v : used)
        {
            cout << v << ' ';
        }
        cout << endl;
        
        if( used.size() > best )
            best = used.size();
    }
    
    cout << best+d << '\n';
}














































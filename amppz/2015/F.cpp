// (10:25) 10:28-10:40

#include <bits/stdc++.h>
using namespace std;

template<typename T>
inline ostream& operator<< (ostream& out, const vector<T>& data) {
    out << data[0];
    for( auto it = data.begin()+1; it != data.end(); it++ )
        out << ' ' << *it;
    return out;
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

template<typename T>
class vector_from_one : public vector<T> {
    public:
        using vector<T>::vector;
        T& operator[] (size_t n) { return vector<T>::operator[](n-1); }
        const T& operator[] (size_t n) const { return vector<T>::operator[](n-1); }
        T& at (size_t n) { return vector<T>::at(n-1); }
        const T& at (size_t n) const { return vector<T>::at(n-1); }
};

const char* const types = "()[]{}";
void test() {
    string input, prefix, sufix;
    cin >> input;
    
    for( char c : input ) {
        int type = 0;
        while( types[type] != c )
            type++;
            
        if( type%2 == 0 )
            sufix.push_back( types[type+1] );
        else {
            if( sufix.empty() )
                prefix.push_back( types[type-1] );
            else if( sufix.back() != c ) {
                cout << "NIE\n";
                return;
            } else
                sufix.pop_back();
        }
    }
    reverse( prefix.begin(), prefix.end() );
    cout << prefix << input << sufix << '\n';
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
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

template<typename A, typename B>
inline istream& operator>> (istream& in, pair<A, B>& data) {
    return in >> data.first >> data.second;
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

int gcd(int a, int b) { return b == 0 ? a : gcd(b, a%b); }

void test();

int main_one() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    test();
    return 0;
}

int main_many() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int T;
    cin >> T;
    while( T --> 0 )
        test();
    return 0;
}

class Double {
    private:
        double value;
    public:
        inline Double() {}
        inline Double(double value) : value(value) {}
        inline operator double() const { return value; }
        friend inline istream& operator>> (istream& in, Double &val) {
            double x = 0.0;
            int ch;

            do {
                ch = in.get();
            } while(isspace(ch));
            bool neg = false;
            if( ch == '-' ) {
                neg = true;
                ch = in.get();
            }

            for( ; isdigit(ch); ch = in.get() )
                x = 10.0 * x + (ch - '0');

            if(ch == '.') {
                for( double mul = 1.0; isdigit(ch = in.get()); )
                    x += (mul *= 0.1) * (ch - '0');
            }

            if(neg) x = -x;
            val.value = x;
            
            return in;
        }
};

/* --------------- </biblioteczka> --------------------- */

int main() { return main_one(); }

void test() {
    int n, k;
    cin >> n >> k;
    vector<unsigned long long> answer(n+1, 0);
    answer[0] = 1;
    for( int pos = 0; pos < n; pos++ )
        for( int step = 1; step <= k and pos+step <= n; step++ )
            answer[pos+step] += answer[pos];
    cout << answer.back() << endl;
}


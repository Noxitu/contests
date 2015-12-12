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
            static char buffer[32];
            char *c = buffer;
            do {
                *c = in.get();
            } while( *c <= 32 or *c >= 128 );
            while( *c > 32 and *c < 128 ) {
                c++;
                *c = in.get();
            }
            *c = '\0';
            val = strtod(buffer, nullptr);
            return in;
        }
};

int main() { cout << setprecision(4) << fixed; return main_many(); }
constexpr double pi = 4*atan(1);

void test() {
    int n, k;
    cin >> n >> k;
    pair<Double, Double> prev, first, next;
    
    double border = pi*k;
    double inner = 0;
    cin >> first;
    prev = first;
    for( int i = 1; i < n; i++ ) {
        cin >> next;
        border += hypot( next.first-prev.first, next.second-prev.second);
        inner += (next.first-prev.first)*(next.second+prev.second);
        prev = next;
    }
    border += hypot( first.first-prev.first, first.second-prev.second);
    inner += (first.first-prev.first)*(first.second+prev.second);

    cout << border*k+fabs(inner)/2 << '\n';
}


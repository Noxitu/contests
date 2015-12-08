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
#ifdef use_iostream
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
#endif
    int T;
#ifdef use_iostream
    cin >> T;
#else
    scanf("%d", &T); 
#endif
    while( T --> 0 )
        test();
    return 0;
}

inline istream& operator>> (istream& in, pair<double, double>& data) {
    string str;
    in >> str;
    data.first = stod(str);
    in >> str;
    data.second = stod(str);
    return in;
}

int main() { cout << setprecision(5) << fixed; return main_many(); }
constexpr double pi = 4*atan(1);

void test() {
    int n, k;
#ifdef use_iostream
    cin >> n >> k;
#else
    scanf("%d %d", &n, &k);
#endif

    pair<double, double> prev, first, next;
    
    double border = pi*k*k;
    double inner = 0;
    for( int i = 0; i <= n; i++ ) {
        if( i == n )
            next = first;
        else
#ifdef use_iostream
            cin >> next;
#else
            scanf("%lf %lf", &next.first, &next.second);
#endif
        if( i == 0 )
            first = next;
        else {
            border += hypot( next.first-prev.first, next.second-prev.second)*k;
            inner += .5*(next.first-prev.first)*(next.second+prev.second);
        }
        prev = next;
    }

#ifdef use_iostream    
    cout << border+fabs(inner) << '\n';
#else
    printf("%lf\n", border+fabs(inner) );
#endif
}


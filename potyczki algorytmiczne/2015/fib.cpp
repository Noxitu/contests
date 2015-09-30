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
inline istream& operator>> (istream& in, vector<T>& data) {
    for( auto &v : data )
        in >> v;
    return in;
}

struct uint128_t {
    uint64_t hi, lo;
    
    uint128_t(uint64_t v) : hi(0), lo(v) {}
    uint128_t(uint64_t hi, uint64_t lo) : hi(hi), lo(lo) {}
    
    explicit operator uint64_t() const { return lo; }
    
    friend uint128_t operator<< (const uint128_t a, int n) {
        uint128_t ret {a.hi<<1, a.lo<<1};
        if( a.lo bitand (1ULL<<63) )
            ret.hi++;
        return ret;
    }
    
    friend uint128_t operator>> (const uint128_t a, int n) {
        uint128_t ret {a.hi>>1, a.lo>>1};
        if( a.hi bitand 1 )
            ret.lo |= (1ULL<<63);
        return ret;
    }
    
    friend uint128_t operator+(const uint128_t a, const uint128_t b) {
        uint128_t ret {a.hi+b.hi, a.lo+b.lo};
        if( a.lo+b.lo < a.lo or a.lo+b.lo < b.lo )
            ret.hi++;
        return ret;
    }
    
    friend uint128_t operator-(const uint128_t a, const uint128_t b) {
        uint128_t ret {a.hi-b.hi, a.lo-b.lo};
        if( a.lo < b.lo )
            ret.hi--;
            
        return ret;
    }
    
    friend uint128_t operator*(uint128_t a, uint64_t b) {
        uint128_t ret {0, 0};
        while( b ) {
            if( b%2 == 1 ) {
                ret = ret + a;
            }
            a = a + a;
            b /= 2;
        }
        return ret;
    }
    
    friend bool operator<(const uint128_t a, const uint128_t b) {
        if( a.hi != b.hi )
            return a.hi < b.hi;
        return a.lo < b.lo;
    }
    
    friend bool operator>=(const uint128_t a, const uint128_t b) {
        return not (a < b);
    }
    
    friend uint128_t operator%(uint128_t a, const uint128_t b) {
        uint128_t c = b;
        while( a >= c )
            c = c << 1;
            
        while( c >= b ) {
            if( a >= c ) {
                a = a - c;
            }
            c = c >> 1;
        }
        
        return a;
    }
    
    friend ostream& operator<< (ostream& out, const uint128_t& a) {
        return out << '(' << a.hi << ", " << a.lo << ')';
    }
};

struct matrix_t {
    uint64_t a, b, c;
    
    static const matrix_t one, base;
    
    friend matrix_t mul(const matrix_t& x, const matrix_t& y, const uint64_t mod) {
        return matrix_t {
            (uint64_t)((uint128_t(x.a)*y.a + uint128_t(x.b)*y.b)%uint128_t(mod)),
            (uint64_t)((uint128_t(x.a)*y.b + uint128_t(x.b)*y.c)%uint128_t(mod)),
            (uint64_t)((uint128_t(x.b)*y.b + uint128_t(x.c)*y.c)%uint128_t(mod))
        };
    }
};
const matrix_t matrix_t::one {1, 0, 1}, matrix_t::base {0, 1, 1};

const auto fib_init = pair<uint64_t, uint64_t>( 0, 1 );
pair<uint64_t, uint64_t> fib(uint64_t n, uint64_t mod) {
    matrix_t ret = matrix_t::one, val = matrix_t::base;
    while( n ) {
        if( n%2 == 1 )
            ret = mul( ret, val, mod );
        val = mul( val, val, mod );
        n /= 2;
    }
    return make_pair(ret.b, ret.c);
}

inline uint64_t stoll(string s) {
    istringstream in(s);
    uint64_t ret;
    in >> ret;
    return ret;
}

void test() {
    string Cstr;
    cin >> Cstr;
    
    uint64_t C = stoll(Cstr);
    
    uint64_t mod = 1;
    uint64_t cycle_length = 1;
    vector<uint64_t> begins;
    begins.push_back(0);
    
    for( int i = 0; i < Cstr.size(); i++ ) {
        mod *= 10;
        uint64_t Cmod = C%mod;
        
        uint64_t new_cycle_length = cycle_length;
        
        vector<uint64_t> new_begins;
        for( auto& begin : begins )
            if( fib(begin, mod).first == Cmod )
                new_begins.push_back(begin);
        
        while( fib(new_cycle_length, mod) != fib_init ) {
            for( auto begin : begins ) {
                begin += new_cycle_length;
                if( fib(begin, mod).first == Cmod )
                    new_begins.push_back(begin);
            }
        
            new_cycle_length += cycle_length;
        }
            
        cycle_length = new_cycle_length;
        begins.swap(new_begins);
        
        if( begins.size() == 0 ) {
            cout << "NIE\n";
            return;
        }
    }
        
    cout << begins.front()+cycle_length << endl;
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
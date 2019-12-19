#include <bits/stdc++.h>
// #define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define MEMORY_LIMIT 256
#define TIME_LIMIT 15.0
// #define ADDITIONAL_LOGS // enable blocks used only for cerr

using namespace std;

/******************************************************************************/
/*********************************
****  Common::fastio v0.1.2  *****
*********************************/
#include <bits/stdc++.h>

namespace common {
    namespace fastio {
        class ostream
        {
            template<typename T> void handle_negative(T &value)
            {
                if(value < T(0)) // depend on compiler optimizing this into false for unsigned
                {
                    putchar_unlocked('-');
                    value = -value;
                }
            }
        public:
            template<typename T, typename SFINAE=typename std::enable_if<std::is_integral<T>::value>::type>
            ostream& operator<<(T value) {
                constexpr size_t size = std::numeric_limits<T>::digits10+1;
                char buffer[size], *pend = buffer+size, *ptr = pend;
                handle_negative(value);
                do {
                    *--ptr = value%10 + '0';
                    value /= 10;
                } while(value);
                
                std::for_each(ptr, pend, putchar_unlocked);
                
                return *this;
            }
            
            ostream& operator<<(std::string const &value) {
                std::for_each(begin(value), end(value), putchar_unlocked);
                return *this;
            }
            
            ostream& operator<<(char c) {
                putchar_unlocked(c);
                return *this;
            }
        } cout;
        
        class istream
        {
            bool valid = true;
        public:
            template<typename T>
            class handle_negative
            {
                T &value;
                bool negative = false;
            public:
                handle_negative(T &value, int &c) : value(value)
                {
                    if(std::is_same<T, typename std::make_signed<T>::type>::value and c == '-')
                    {
                        c = getchar_unlocked();
                        negative = true;
                    }
                }
                ~handle_negative()
                {
                    if(std::is_same<T, typename std::make_signed<T>::type>::value and negative)
                    {
                        value = -value;
                    }
                }
            };
        
            template<typename T, typename SFINAE=typename std::enable_if<std::is_integral<T>::value>::type>
            istream& operator>>(T &value) {
                int c = 0;
                while(0 <= c and c <= 32) c = getchar_unlocked();
                handle_negative<T> _(value, c);
                value = 0;
                while(c > 32) {
                    value = 10*value + c-'0';
                    c = getchar_unlocked();
                }
                valid = (c != -1);
                return *this;
            }
            operator bool() const { return valid; }
        } cin;
        
        template<typename T>
        class istream_iterator
        {
            T value;
            bool valid;
            istream *in;
        public:
            typedef void difference_type;
            typedef T value_type;
            typedef T *pointer;
            typedef T &reference;
            typedef input_iterator_tag iterator_category;

            istream_iterator() : valid(false) {}
            istream_iterator(istream &in) : in(&in), valid(in >> value) {}
            friend bool operator==(istream_iterator const &a, istream_iterator const &b) { return a.valid == b.valid; }
            friend bool operator!=(istream_iterator const &a, istream_iterator const &b) { return a.valid != b.valid; }
            istream_iterator& operator ++() { valid = (*in >> value); return *this; }
            value_type operator*() const { return value; }
        };
        
        template<typename T>
        class ostream_iterator
        {
            const std::string separator;
            bool need_sep = false;
            ostream &out;
        public:
            typedef void difference_type;
            typedef T value_type;
            typedef T *pointer;
            typedef T &reference;
            typedef output_iterator_tag iterator_category;

            ostream_iterator(ostream &out, std::string separator="") : out(out), separator(separator) {}
            friend bool operator==(ostream_iterator const &a, ostream_iterator const &b) { return true; }
            friend bool operator!=(ostream_iterator const &a, ostream_iterator const &b) { return false; }
            ostream_iterator& operator ++() { return *this; }
            
            struct proxy {
                ostream_iterator &it;
                T operator=(T value) {
                    if(it.need_sep)
                        it.out << it.separator;
                    else
                        it.need_sep = true;
                    it.out << value;
                }
            };
            proxy operator*() { return proxy{*this}; }
        };
    }
}

// end of #include <fastio.hpp>
//====================================================

constexpr long long MOD = 2'500'000'001;

void test()
{
    using common::fastio::cin;

    auto no = []() { cout << "NO SOLUTION\n"; };

    int n;
    long long m;
    cin >> n >> m;

    const long long D = m*m*m*m/8/8;
    long long p = 1;

    long long answer = 1;

    while (n --> 0)
    {
        long long a;
        cin >> a;

        if (a <= D)
        {
            while (a%p != 0 || a/p > p+m)
            {
                p += 2;

                if (p > a/p)
                    return no();
            }

            answer = p * answer % MOD;
        }
        else
        {
            const long long A = ceil(sqrt(a));
            const long long B = sqrt(A*A-a);
            const long long new_p = A-B;

            if (new_p < p)
                return no();

            p = new_p;

            if (a%p != 0 || a/p > p+m)
                return no();

            answer = p * answer % MOD;

        }
    }

    cout << answer << '\n';
}

/******************************************************************************/

int main()
{
    using common::fastio::cin;

#if !defined(CONTEST_WORKSPACE)
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
#endif

#if defined(MULTIPLE_TESTS)
    int T = 0;
    cin >> T;

    while (T --> 0)
        test();

#elif defined(ENDLESS_TESTS)
    while(!(cin >> std::ws).eof())
        test();

#else
    test();
#endif

    return EXIT_SUCCESS;
}

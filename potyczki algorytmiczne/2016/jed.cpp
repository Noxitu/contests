/*********************************
*****     Common v2.0.6      *****
*********************************/
#include <bits/stdc++.h>

void test();

namespace common {

    template<typename T>
    T gcd(T const &a, T const &b) { return b == 0 ? a : gcd(b, a%b); }


    namespace io {
        template<typename T>
        inline std::ostream& operator<< (std::ostream& out, const std::vector<T>& data) {
            out << data[0];
            for( auto it = data.begin()+1; it != data.end(); it++ )
                out << ' ' << *it;
            return out;
        }

        template<typename T>
        inline std::istream& operator>> (std::istream& in, std::vector<T>& data) {
            for( auto &v : data )
                in >> v;
            return in;
        }

        template<typename A, typename B>
        inline std::ostream& operator<< (std::ostream& out, const std::pair<A, B>& data) {
            return out << data.first << ' ' << data.second;
        }

        template<typename A, typename B>
        inline std::istream& operator>> (std::istream& in, std::pair<A, B>& data) {
            return in >> data.first >> data.second;
        }
    }

    template<typename T>
    class vector_from_one : public std::vector<T> {
        public:
            using std::vector<T>::vector;
            T& operator[] (size_t n) { return std::vector<T>::operator[](n-1); }
            const T& operator[] (size_t n) const { return std::vector<T>::operator[](n-1); }
            T& at (size_t n) { return std::vector<T>::at(n-1); }
            const T& at (size_t n) const { return std::vector<T>::at(n-1); }
    };


    namespace functional {    
        template<typename Type, typename C> struct GetComparator { using Comparator = C; };
        template<typename Type> struct GetComparator<Type, void> { using Comparator = std::less<Type>; };

        template<typename Object, typename Type, Type Object::* default_field, typename Comparator = void>
        struct CompareField
        {
            const Type Object::* field;

            CompareField() : field(default_field) {}
            CompareField(Type Object::* field) : field(field) {}
            constexpr bool operator()(Object const &lhs, Object const &rhs) const
            {
                return typename GetComparator<Type, Comparator>::Comparator()(lhs.*field, rhs.*field);
            }
        };

        template<typename Comparator = void, typename Object, typename Type>
        const CompareField<Object, Type, nullptr, Comparator> compare_field(Type Object::* field) { return {field}; }

        template<typename iterator_type>
        class Iterable
        {
            typedef iterator_type iterator;
            const iterator begin_iterator;
            const iterator end_iterator;
        public:
            Iterable(iterator begin, iterator end) : begin_iterator(begin), end_iterator(end) {}
            
            iterator begin() { return begin_iterator; }
            iterator end() { return end_iterator; }
        };

        template<typename iterator>
        Iterable<iterator> iterable(iterator begin, iterator end){ return Iterable<iterator>(begin, end); }

        template<typename Collection>
        auto reversed(Collection &collection) -> Iterable<decltype(collection.rbegin())> { return iterable(collection.rbegin(), collection.rend()); }
    }

    
    namespace operators {
        struct base_operator {};
        template <typename LHS, typename OP> struct operator_proxy { LHS lhs; OP op; };

        template <typename LHS, typename OP, typename SFINAE = typename std::enable_if<std::is_base_of<base_operator, typename std::remove_reference<OP>::type>::value>::type>
        operator_proxy<LHS, OP> operator<(LHS &&lhs, OP &&op) { return {lhs, op}; }

        template <typename LHS, typename OP, typename RHS>
        auto operator>(operator_proxy<LHS, OP> proxy, RHS &&rhs) -> decltype(proxy.op(proxy.lhs, rhs)) { proxy.op(proxy.lhs, rhs); }

        struct : public base_operator { template <typename T> void operator()(T &x, T y) const { x = max(x, y); } } const set_if_greater;
        struct : public base_operator { template <typename T> void operator()(T &x, T y) const { x = min(x, y); } } const set_if_less;
    }
    

    namespace main {
        int _default(int const, char const * const[])
        {
            std::cout << "Undefined common::main app!" << std::endl;
            return 1;
        }

        int (*_app)(int const, char const * const[]);

        class one
        {
            static int main(int const argc, char const * const argv[])
            {
                std::ios_base::sync_with_stdio(false);
                std::cin.tie(NULL);
                test();
                return 0;
            }
        public:
            one() { _app = main; }
        };

        class many
        {
            static int main(int const argc, char const * const argv[])
            {
                std::ios_base::sync_with_stdio(false);
                std::cin.tie(NULL);
                int T;
                std::cin >> T;
                while( T --> 0 )
                    test();
                return 0;
            }
        public:
            many() { _app = main; }
        };

        class all
        {
            static int main(int const argc, char const * const argv[])
            {
                std::ios_base::sync_with_stdio(false);
                std::cin.tie(NULL);
                std::cin.exceptions(std::ifstream::eofbit);
                try {
                    while(std::cin)
                        test();
                }
                catch (std::ifstream::failure&) {}
                return 0;
            }
        public:
            all() { _app = main; }
        };
    }
}

int main(int const argc, char const * const argv[])
{
    return common::main::_app(argc, argv);
}

using namespace std;
using namespace common;
using namespace common::io;
using namespace common::functional;
using namespace common::operators;

// end of #include <common.hpp>
//====================================================

main::many _;

void print(int n)
{
    if(n == 1)
    {
        cout << '1';
        return;
    }

    if(n <= 3)
    {
        cout << "1+";
        print(n-1);
        return;
    }

    cout << "(1+1)*(";
    print(n/2);
    cout << ')';
    if(n&1)
        cout << "+1";
}

void test()
{
    int k;
    cin >> k;
    print(k);
    cout << '\n';
}
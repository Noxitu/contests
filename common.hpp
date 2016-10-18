/*********************************
*****     Common v2.0.1      *****
*********************************/
#include <bits/stdc++.h>

void test();

namespace common {
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

    int gcd(int a, int b) { return b == 0 ? a : gcd(b, a%b); }

    namespace functional {
        template<class Object, typename Type, Type Object::* Field, typename Comparator=std::less<Type>>
        struct compare_field
        {
            constexpr bool operator()(Object const &lhs, Object const &rhs)
            {
                return Comparator()(lhs.*Field, rhs.*Field);
            }
        };

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
 
    namespace main {
        int main_one() {
            std::ios_base::sync_with_stdio(false);
            std::cin.tie(NULL);
            test();
            return 0;
        }

        int main_many() {
            std::ios_base::sync_with_stdio(false);
            std::cin.tie(NULL);
            int T = 1;
            std::cin >> T;
            while( T --> 0 )
                test();
            return 0;
        }
    }
}

using namespace std;
using namespace common;
using namespace common::functional;
using namespace common::main;

// end of #include <common.hpp>
//====================================================
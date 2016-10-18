/*********************************
*****     Common v2.0.0      *****
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
        auto reversed(Collection &collection) -> decltype(collection.rbegin()) { return iterable(collection.rbegin(), collection.rend()); }
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

int main() { return main_many(); }

constexpr int no_label = -2;
constexpr int inf = -1;

struct Peasant
{
    int label = no_label;
    int mex;
    
    std::vector<reference_wrapper<Peasant>> bosses;
    std::vector<reference_wrapper<Peasant>> underlings;
    vector<bool> used;
    Peasant(int n) : used(n, false) {}
};

void test()
{
    int n, m;
    cin >> n >> m;
    
    vector_from_one<Peasant> peasants(n, n);
    
    while( m --> 0 )
    {
        int a, b;
        cin >> a >> b;
            
        peasants[b].bosses.push_back(peasants[a]);
        peasants[a].underlings.push_back(peasants[b]);
    }
    
    for(int i = 0, unlabeled = n; unlabeled > 0; ++i)
    {
        auto check_peasant = [i](Peasant &u)
        { 
            auto check_underling = [i, &u](Peasant const &w){ return &u == &w or w.label != no_label and w.label != inf or w.used[i] == true; };
            return u.label == no_label and not u.used[i] and all_of(u.underlings.begin(), u.underlings.end(), check_underling);
        };

        
        for(auto it = peasants.end(); (it = find_if(peasants.begin(), peasants.end(), check_peasant)) != peasants.end();)
        {
            it->label = i;
            --unlabeled;
            for(Peasant &w : it->bosses)
                w.used[i] = true;
        }
        
        for(Peasant &u : peasants)
            if(u.label == no_label and not u.used[i])
            {
                u.label = inf;
                --unlabeled;
            }
    }
    for(auto &u : peasants)
    {
        cout << u.label << '\n';
    }
}


























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
/**********************************
 *  Common :: union_find  v1.0.3  *
 **********************************/

namespace common
{
    namespace union_find
    {
        class Base
        {
            friend class AccessBase;
            Base *parent = nullptr;
            size_t rank = 0;
        };

        struct AccessBase {
            static Base*& get_parent(Base &base) { return base.parent; }
            static Base* const& get_parent(Base const &base) { return base.parent; }
            static size_t& get_rank(Base &base) { return base.rank; }
        };

        namespace impl
        {
            template<typename Type, size_t length, typename SFINAE = typename std::enable_if<std::is_integral<Type>::value>::type>
            struct array
            {
                constexpr static bool simple_constructor = true;

                struct Data
                {
                    Type parent = length;
                    size_t rank = 0;
                };

                typedef Type Argument;
                typedef std::array<Data, length> DataContainer;

                constexpr static bool has_parent(Argument arg, DataContainer const &data) { return data[arg].parent != length; }
                constexpr static Argument get_parent(Argument arg, DataContainer const &data) { return data[arg].parent; }
                constexpr static void set_parent(Argument arg, Argument parent, DataContainer &data) { data[arg].parent = parent; }
                constexpr static void reset_parent(Argument arg, DataContainer &data) { data[arg].parent = length; }
                constexpr static size_t& rank(Argument arg, DataContainer &data) { return data[arg].rank; }
                constexpr static bool eq(Argument a, Argument b) { return a == b; }
            };

            template<typename Type, typename SFINAE = typename std::enable_if<std::is_integral<Type>::value>::type>
            struct vector
            {
                constexpr static bool simple_constructor = false;

                struct Data
                {
                    Type parent;
                    size_t rank = 0;
                    Data(Type rhs) : parent(rhs) {}
                };

                typedef Type Argument;
                typedef std::vector<Data> DataContainer;

                constexpr static bool has_parent(Argument arg, DataContainer const &data) { return data[arg].parent != data.size(); }
                constexpr static Argument get_parent(Argument arg, DataContainer const &data) { return data[arg].parent; }
                constexpr static void set_parent(Argument arg, Argument parent, DataContainer &data) { data[arg].parent = parent; }
                constexpr static void reset_parent(Argument arg, DataContainer &data) { data[arg].parent = arg; }
                constexpr static size_t& rank(Argument arg, DataContainer &data) { return data[arg].rank; }
                constexpr static bool eq(Argument a, Argument b) { return a == b; }
            };

            template<typename Type, typename SFINAE = typename std::enable_if<not std::is_base_of<Base, Type>::value>::type>
            struct map
            {
                constexpr static bool simple_constructor = true;

                struct Data
                {
                    Type parent;
                    size_t rank = 0;
                    Data(Type rhs) : parent(rhs) {}
                };

                typedef Type Argument;
                typedef std::map<Type, Data> DataContainer;

                constexpr static bool has_parent(Argument arg, DataContainer const &data) { return data.find(arg) != data.end() and data.at(arg).parent != arg; }
                constexpr static Argument get_parent(Argument arg, DataContainer const &data) { return data.at(arg).parent; }
                constexpr static void set_parent(Argument arg, Argument parent, DataContainer &data) 
                {
                    auto it = data.find(arg);
                    if( it != data.end() )
                        it->second.parent = parent;
                    else
                        data.insert(std::pair<Type, Data>(arg, parent));
                }
                constexpr static void reset_parent(Argument arg, DataContainer &data) { data.erase(arg); }
                static size_t& rank(Argument arg, DataContainer &data)
                {
                    auto it = data.find(arg);
                    if( it != data.end() )
                        return it->second.rank;
                    
                    data.insert(std::pair<Type, Data>(arg, arg));
                    return data.at(arg).rank;
                }
                constexpr static bool eq(Argument a, Argument b) { return a == b; }
            };

            template<typename Type, typename SFINAE = typename std::enable_if<std::is_base_of<Base, Type>::value>::type>
            struct object
            {
                constexpr static bool simple_constructor = true;

                typedef Type &Argument;
                typedef struct {} DataContainer;

                constexpr static bool has_parent(Argument const arg, DataContainer) { return AccessBase::get_parent(arg); }
                constexpr static Argument get_parent(Argument const arg, DataContainer) { return static_cast<Argument>(*AccessBase::get_parent(arg)); }
                constexpr static void set_parent(Argument arg, Argument parent, DataContainer) { AccessBase::get_parent(arg) = &parent; }
                constexpr static void reset_parent(Argument arg, DataContainer) { AccessBase::get_parent(arg) = nullptr; }
                constexpr static size_t& rank(Argument arg, DataContainer) { return AccessBase::get_rank(arg); }
                constexpr static bool eq(Argument const a, Argument const b) { return &a == &b; }
            };
        }


        template<typename Type, typename Impl>
        class UnionFind
        {
            typedef typename Impl::Argument Argument;
        public: 
            typename Impl::DataContainer data;
        public:
            template<bool TRUE = true, typename SFINAE = typename std::enable_if<TRUE and Impl::simple_constructor>::type>
            UnionFind() {}

            template<bool TRUE = true, typename SFINAE = typename std::enable_if<TRUE and not Impl::simple_constructor>::type>
            UnionFind(size_t length) : data(length, length) {}

            Argument find(Argument arg)
            {
                if(not Impl::has_parent(arg, data))
                    return arg;

                Argument root = find(Impl::get_parent(arg, data));
                Impl::set_parent(arg, root, data);
                return root;
            }

            void join(Argument a, Argument b)
            {
                Argument a_root = find(a);
                Argument b_root = find(b);

                if(Impl::rank(a_root, data) > Impl::rank(b_root, data))
                    Impl::set_parent(b_root, a_root, data);
                else if(Impl::rank(a_root, data) < Impl::rank(b_root, data))
                    Impl::set_parent(a_root, b_root, data);
                else if(not Impl::eq(a_root, b_root))
                {
                    Impl::set_parent(b_root, a_root, data);
                    ++Impl::rank(a_root, data);
                }
            }

            bool joined(Argument a, Argument b)
            {
                return Impl::eq(find(a), find(b));
            }

            void reset(Argument a)
            {
                Impl::reset_parent(a, data);
                Impl::rank(a, data) = 0;
            }
        };


        template<typename Type, size_t length>
        UnionFind<Type, impl::array<Type, length>> create() { return {}; }

        template<typename Type>
        UnionFind<Type, impl::vector<Type>> create(size_t length) { return {length}; }

        template<typename Type>
        UnionFind<Type, impl::object<Type>> create() { return {}; }

        template<typename Type>
        UnionFind<Type, impl::map<Type>> create() { return {}; }
    }
}

// end of #include <union_find.hpp>
//==================================

main::one _;

struct Employee;
typedef reference_wrapper<Employee> EmployeeRef;
typedef vector<EmployeeRef>::iterator EmployeesIterator;
typedef Iterable<EmployeesIterator> EmployeesView;

struct Employee : union_find::Base 
{
    int id;
    EmployeesIterator begin, end;
    bool allowed_as_root;
    bool in_tree = false;
    int chef = 0;
};

typedef pair<EmployeeRef, EmployeeRef> Edge;
typedef vector<Edge> Edges;

void test()
{
    int n, m;
    cin >> n >> m;

    vector_from_one<Employee> employees(n);
    for(int i = 1; i <= n; ++i)
        employees[i].id = i;

    vector<EmployeeRef> employees_tree(begin(employees), end(employees));

    Edges yes, no;
    yes.reserve(m);
    no.reserve(m);

    while( m --> 0 )
    {
        int a, b;
        char type;
        cin >> a >> b >> ws >> type;
        if(type == 'T')
        {
            yes.emplace_back(employees[a], employees[b]);
            no.emplace_back(employees[b], employees[a]);
        }
        else
        {
            no.emplace_back(employees[a], employees[b]);
        }
    }

    std::vector<EmployeesView> subtrees = {iterable(begin(employees_tree), end(employees_tree))};

    while(not subtrees.empty())
    {
        auto uf = union_find::create<Employee>();

        for(Employee &emp : employees)
        {
            emp.allowed_as_root = true;
            uf.reset(emp);
        }

        for(auto &edge : no)
            edge.second.get().allowed_as_root = false;

        for(auto &subtree : subtrees)
        {
            EmployeesIterator root_it = find_if( begin(subtree), end(subtree), [](Employee &emp){ return emp.allowed_as_root; } );

            if(root_it == end(subtree))
            {
                cout << "NIE\n";
                return;
            }

            swap(*root_it, *begin(subtree));
            Employee &root = *begin(subtree);
            
            root.begin = next(begin(subtree));
            root.end = end(subtree);
            root.in_tree = true;

            for(Employee &e : iterable(root.begin, root.end))
                e.chef = root.id;
        }

        {
            auto invalid_edge = [](Edge &edge) { return edge.first.get().in_tree or edge.second.get().in_tree; };
            yes.erase(remove_if(begin(yes), end(yes), invalid_edge), end(yes));
        }

        for(auto &edge : yes)
        {
            uf.join(edge.first, edge.second);
        }

        std::vector<EmployeesView> new_subtrees;
        for(auto &subtree : subtrees)
        {
            Employee &root = *begin(subtree);

            sort(root.begin, root.end, [&uf](Employee &a, Employee &b) { return uf.find(a).id < uf.find(b).id; });
            for(EmployeesIterator it1 = root.begin, it2; it1 != root.end; it1 = it2)
            {
                it2 = find_if(it1, root.end, [&uf, it1](Employee &e) { return not uf.joined(*it1, e); });
                new_subtrees.emplace_back(it1, it2);
            }
        }
        subtrees.swap(new_subtrees);

        {
            auto invalid_edge = [&uf](Edge &edge) { return not uf.joined(edge.first, edge.second); };
            no.erase(remove_if(begin(no), end(no), invalid_edge), end(no));
        }
    }

    //cout << "TAK\n";
    for(Employee &e : employees)
        cout << e.chef << '\n';
}
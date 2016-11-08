#include <bits/stdc++.h>
/**********************************
 *  Common :: union_find  v1.0.0  *
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
                typedef void* DataContainer[0];

                constexpr static bool has_parent(Argument const arg, DataContainer) { return AccessBase::get_parent(arg); }
                constexpr static Argument get_parent(Argument const arg, DataContainer) { return static_cast<Argument>(*AccessBase::get_parent(arg)); }
                constexpr static void set_parent(Argument arg, Argument parent, DataContainer) { AccessBase::get_parent(arg) = &parent; }
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

            void union_with(Argument a, Argument b)
            {
                Argument a_root = find(a);
                Argument b_root = find(b);

                if(Impl::rank(a_root, data) > Impl::rank(b_root, data))
                    Impl::set_parent(b_root, a_root, data);
                else if(Impl::rank(a_root, data) < Impl::rank(b_root, data))
                    Impl::set_parent(a_root, b_root, data);
                else if(a_root != b_root)
                {
                    Impl::set_parent(b_root, a_root, data);
                    ++Impl::rank(a_root, data);
                }
            }

            bool joined(Argument a, Argument b)
            {
                return find(a) == find(b);
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
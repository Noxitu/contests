/*********************************
****  Common::fastio v0.0.1  *****
*********************************/
#include <bits/stdc++.h>

namespace common {
    namespace fastio {
        struct my_traits : public std::char_traits<char> {};
    }
}

namespace std
{   
    template<>
    class basic_streambuf<char, common::fastio::my_traits>
    {
        int buf;
    public:
        typedef char char_type;
        typedef common::fastio::my_traits traits_type;
        typedef traits_type::int_type int_type;		
        typedef traits_type::pos_type pos_type;
        typedef traits_type::off_type off_type;
        
        basic_streambuf() : buf(getchar_unlocked()) {}
        
        locale pubimbue( const locale& loc ) { return loc; }
        int_type pubsync() { return 0; }
        
        int_type sputc( char_type c )
        {
            putchar_unlocked(c);
            return 0;
        }
        
        streamsize sputn( const char_type* s, streamsize count )
        {
            for_each(s, s+count, putchar_unlocked);
            return count;
        }
        
        int_type sgetc()
        {
            return buf != -1 ? buf : traits_type::eof();
        }
        
        int_type snextc()
        {
            buf = getchar_unlocked();
            return buf != -1 ? buf : traits_type::eof();
        }
        
        int_type sbumpc()
        {
            int_type ret = (buf != -1 ? buf : traits_type::eof());
            buf = getchar_unlocked();
            return ret;
        }
    };
    
    template<>
    class num_put<char, ostreambuf_iterator<char, common::fastio::my_traits>> : public locale::facet
    {
    public:
        typedef char char_type;
        typedef ostreambuf_iterator<char, common::fastio::my_traits> iter_type;
        
        iter_type put( iter_type out, std::ios_base& str, char_type fill, long value ) const
        {
            if(value == 0)
            {
                *out++ = '0';
                return out;
            }
            if(value < 0) {
                *out++ = '-';
                value = -value;
            }
                
            char output[sizeof(value)];
            char *end = output+sizeof(value), *ptr = end;
            while(value) {
                *--ptr = value%10+'0';
                value /= 10;
            }
            return copy(ptr, end, out);
        }
        
        iter_type put( iter_type out, std::ios_base& str, char_type fill, double value ) const
        {
            string s = "<double io not implemented>";
            return std::copy(begin(s), end(s), out);
        }

        static locale::id id;
    };
    
    locale::id num_put<char, ostreambuf_iterator<char, common::fastio::my_traits>>::id;
    
    
    template<>
    class num_get<char, istreambuf_iterator<char, common::fastio::my_traits>> : public locale::facet
    {
    public:
        typedef char char_type;
        typedef istreambuf_iterator<char, common::fastio::my_traits> iter_type;
        
        iter_type get( iter_type in, iter_type end, ios_base& str, ios_base::iostate& err, long& value ) const
        {
            long neg = (*in == '-' ? -1 : 1);
            iter_type it = (neg==1 ? in : next(in));
            value = 0;
            for(; it != end and not isspace(*it); ++it)
                value = value * 10 + *it-'0';
            value *= neg;
            return it;
        }

        static locale::id id;
    };
    
    locale::id num_get<char, istreambuf_iterator<char, common::fastio::my_traits>>::id;
    
    basic_ostream<char, common::fastio::my_traits>& operator<<(basic_ostream<char, common::fastio::my_traits> &out, basic_string<char> const &data) { return out << data.c_str(); }
    basic_istream<char, common::fastio::my_traits>& operator>>(basic_istream<char, common::fastio::my_traits> &in, basic_string<char> &data) {
        basic_string<char, common::fastio::my_traits> str;
        in >> str;
        data.resize(str.size());
        copy(begin(str), end(str), begin(data));
        return in;
    }
}

namespace common {
    namespace fastio {
        namespace internal {
            std::basic_streambuf<char, my_traits> fast_io_buf;
        }
        std::basic_ostream<char, my_traits> cout(&internal::fast_io_buf);
        std::basic_istream<char, my_traits> cin(&internal::fast_io_buf);
        
        template<typename T> using ostream_iterator = std::ostream_iterator<T, char, my_traits>;
        template<typename T> using istream_iterator = std::istream_iterator<T, char, my_traits>;

        namespace internal {
            struct static_initializer {
                static_initializer() {
                    std::locale out_locale(std::locale(), new std::num_put<char, std::ostreambuf_iterator<char, my_traits>>);
                    cout.imbue(out_locale);
                    
                    std::locale in_locale(std::locale(), new std::num_get<char, std::istreambuf_iterator<char, my_traits>>);
                    cin.imbue(in_locale);
                }
            } const static_initializer;
        }
    }
}

// end of #include <fastio.hpp>
//====================================================
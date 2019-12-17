#include <istream>
#include <iostream>
#include <string>
#include <memory>
#include <sstream>

namespace contest_workspace
{
    bool compare(std::istream &output, std::istream &truth)
    {
        while(output || truth)
        {
            std::string lhs, rhs;
            output >> lhs;
            truth >> rhs;

            if (lhs != rhs)
            {
                return false;
            }
        }

        return true;
    }

    class my_streambuf : public std::streambuf
    {
        std::shared_ptr<std::istream> m_in_buffer;
        std::ostream &m_out_buffer;
        std::string m_word_buffer;

        int overflow (int c) override
        {
            if (c == EOF)
                return c;

            m_out_buffer << static_cast<char>(c);

            if (std::isspace(c))
                check_word();
            else
                m_word_buffer.push_back(c);
                
            return c;
        }

        void check_word()
        {
            if (m_word_buffer.empty()) return;

            std::string true_word;
            *m_in_buffer >> true_word;

            if (m_word_buffer != true_word)
            {
                std::stringstream ss;
                ss << "Invalid word in input. Read \"" << m_word_buffer << "\", expected \"" << true_word << "\".";
                std::cerr << "[CONTEST_WORKSPACE] " << ss.str() << std::endl;
                std::cerr << std::flush;
                //throw std::runtime_error(ss.str());
            }

            m_word_buffer.clear();
        }
    public:
        my_streambuf(std::shared_ptr<std::istream> truth, std::ostream &output) : m_in_buffer(truth), m_out_buffer(output) {}

        virtual ~my_streambuf()
        {
            check_word();
        }
    };

    std::unique_ptr<std::streambuf> online_compare(std::shared_ptr<std::istream> truth, std::ostream &output)
    {
        return std::make_unique<my_streambuf>(truth, output);
    }
}
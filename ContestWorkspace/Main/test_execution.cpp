
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <set>
#include <vector>
#include <string>
#include <experimental/filesystem>
#include <experimental/optional>
#include <chrono>

#include "compare.h"
#include "limits.h"
#include "resources.h"
#include "test_execution.h"

namespace contest_workspace
{
    class swap_rdbuf
    {
    private:
        std::ios &m_stream;
        std::streambuf *m_rdbuf;
    public:
        swap_rdbuf(std::ios &stream, std::ios &other) : m_stream(stream)
        {
            m_rdbuf = stream.rdbuf(other.rdbuf());
        }

        swap_rdbuf(std::ios &stream, std::streambuf *other) : m_stream(stream)
        {
            m_rdbuf = stream.rdbuf(other);
        }

        ~swap_rdbuf()
        {
            m_stream.rdbuf(m_rdbuf);
        }
    };

    TestResult run_test(const std::string &test_group_name, const Test &test)
    {
        const std::string full_test_name = test_group_name + "." + test.name;
        std::cout << "[ RUN      ] " << full_test_name << std::endl;

        std::stringstream sout;
        std::ifstream fin(test.input_path);

        const auto time_before = std::chrono::high_resolution_clock::now();

        bool result = [&]()
        {
            try
            {
                std::unique_ptr<std::streambuf> debug_streambuf;

                swap_rdbuf swap_cin(std::cin, fin);
                swap_rdbuf swap_cout = [&]()
                {
    #ifndef NDEBUG
                    if (test.output_path)
                    {
                        debug_streambuf = online_compare(std::make_shared<std::ifstream>(*test.output_path),
                                                        sout);
                        return swap_rdbuf(std::cout, debug_streambuf.get());
                    }
    #endif
                    return swap_rdbuf(std::cout, sout);
                }();

                limit_ram(memory_limit);
                auto time_guard = limit_cpu(time_limit);
                return ::contestworkspace_test_main() == 0;
            }
            catch(const std::bad_alloc&)
            {
                std::cout << "[CONTEST_WORKSPACE] " << "Exceeded Memory Limit!" << std::endl;
                return false;
            }
            catch(const std::exception &ex)
            {
                std::cout << "[CONTEST_WORKSPACE] " << "Exception " << typeid(ex).name() << ": " << ex.what() << std::endl;
                return false;
            }
            catch(...)
            {
                std::cout << "[CONTEST_WORKSPACE] " << "Exception ..." << std::endl;
                return false;
            }
        }();

        const auto time_after = std::chrono::high_resolution_clock::now();

        const int duration = std::chrono::duration_cast<std::chrono::milliseconds>(time_after-time_before).count();

        if (duration == 0)
        {
            const int precise_duration = std::chrono::duration_cast<std::chrono::microseconds>(time_after-time_before).count();
            std::cout << "Precise duration = " << precise_duration << " μs" << std::endl;
        }

        if (result && test.output_path)
        {
            {
                std::ifstream truth(*test.output_path);
                result &= compare(sout, truth);
            }

            if (!result)
            {
                std::ifstream truth(*test.output_path);
                std::string truth_str;
                std::getline(truth, truth_str, '\0');

                std::cout << "Wrong answer: " << std::endl;
                std::cout << sout.str() << std::endl;
                std::cout << "Truth: " << std::endl;
                std::cout << truth_str << std::endl;
            }
        }
        else if (result)
            std::cout << sout.str();

        if (result)
            std::cout << "[       OK ] ";
        else 
            std::cout << "[  FAILED  ] ";

        std::cout << full_test_name << " (" << duration << " ms)" << std::endl;

        return {result, duration};
    }
}

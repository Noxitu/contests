#include "runner.h"
#include <string>
#include <sstream>
#include <functional>
#include <iostream>

template<typename ValueType>
ValueType getParam(int const argc, char const *const argv[], std::string key, ValueType def)
{
    for(int i = 1; i < argc; ++i)
        if(std::string(argv[i]) == key)
        {
            std::stringstream ss(argv[i+1]);
            ValueType ret;
            ss >> ret;
            return ret;
        }
    return def;
}

int thread_main();

int main(int const argc, char const *const argv[])
{
    try
    {
        int nodes = getParam(argc, argv, "--nodes", 10);
        int workers = getParam(argc, argv, "-j", 1);

        Runner runner(nodes, workers, thread_main);

        auto running_since = std::chrono::high_resolution_clock::now();
        runner.run();
        auto running_time = std::chrono::high_resolution_clock::now() - running_since;

        std::cout << "Runner was running for " << std::chrono::duration_cast<std::chrono::milliseconds>(running_time).count() << "ms\n";
        runner.print_stats(std::cout);
    }
    catch(std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        throw;
    }
    catch(...)
    {
        std::cerr << "Unknown exception" << std::endl;
        throw;
    }
}
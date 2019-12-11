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
#include "resources.h"

#include "test_discovery.h"

int contestworkspace_test_main();
bool compare(std::istream &output, std::istream &truth);

std::function<bool(const std::string&, const std::string&)> gtest_filter(const std::vector<std::string> &args)
{
    const std::string prefix = "--gtest_filter=";
    for (auto &arg : args)
    {
        if (arg.substr(0, prefix.size()) != prefix)
            continue;

        std::set<std::string> included_tests;

        auto start = std::find(arg.begin(), arg.end(), '=');
        std::ofstream out("/tmp/log.txt");
        while (start != arg.end())
        {
            auto end = std::find(start+1, arg.end(), ':');
            included_tests.insert(std::string(start+1, end));
            start = end;
        }

        return [included_tests](auto group_name, auto task_name) { 
            return included_tests.count(group_name + "." + task_name) > 0; 
        };
    }
    
    return [](auto, auto) { return true; };
}


bool has_arg(const std::string arg, const std::vector<std::string> &args)
{
    return std::find(args.begin()+1, args.end(), arg) != args.end();
}

void print_help()
{
    std::cout << "This program contains tests written using Google Test." << std::endl;
    std::cout << "contest_workspace::tests_path = " << contest_workspace::tests_path << std::endl;
}

void print_test_list()
{
    const auto tests = discover_tests(contest_workspace::tests_path);

    for (auto &test_group : tests)
    {
        std::cout << test_group.name << "." << std::endl;
        for (auto &test : test_group.tests)
            std::cout << "  " << test.name << std::endl;
    }
}

struct TestResult
{
    bool result;
    int duration;
};

TestResult run_test(const std::string &test_group_name, const Test &test)
{
    const std::string full_test_name = test_group_name + "." + test.name;
    std::cout << "[ RUN      ] " << full_test_name << std::endl;

    std::stringstream sout;
    std::ifstream fin(test.input_path);

    auto orig_cin_rdbuf = std::cin.rdbuf(fin.rdbuf());
    auto orig_cout_rdbuf = std::cout.rdbuf(sout.rdbuf());

    const auto time_before = std::chrono::high_resolution_clock::now();
    bool result = ::contestworkspace_test_main() == 0;
    const auto time_after = std::chrono::high_resolution_clock::now();

    std::cin.rdbuf(orig_cin_rdbuf);
    std::cout.rdbuf(orig_cout_rdbuf);

    const int duration = std::chrono::duration_cast<std::chrono::milliseconds>(time_after-time_before).count();

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
    else
        std::cout << sout.str();

    if (result)
        std::cout << "[       OK ] ";
    else 
        std::cout << "[  FAILED  ] ";

    std::cout << full_test_name << " (" << duration << " ms)" << std::endl;

    return {result, duration};
}

int main(const int argc, const char *argv[])
{
    std::vector<std::string> args(argv, argv+argc);

    if (args.size() <= 1)
    {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(nullptr);
        return contestworkspace_test_main();
    }

    if (has_arg("--help", args))
    {
        print_help();
        return EXIT_SUCCESS;
    }
    
    if (has_arg("--gtest_list_tests", args))
    {
        print_test_list();
        return EXIT_SUCCESS;
    }

    if (has_arg("--gtest_color=no", args) || has_arg("--test", args))
    {
        auto tests = discover_tests(contest_workspace::tests_path);
        auto filter = gtest_filter(args);

        int max_duration = 0;
        double total_duration = 0.0;
        int total = 0;
        int oks = 0;
        int fails = 0;

        for (auto &test_group : tests)
            for (auto &test : test_group.tests)
            {
                if (!filter(test_group.name, test.name))
                    continue;

                const auto result = run_test(test_group.name, test);

                total += 1;
                if (result.result)
                    oks += 1;
                else
                    fails += 1;

                total_duration += result.duration / 1000.;
                max_duration = std::max(max_duration, result.duration);
            }

        if (has_arg("--test", args))
        {
            std::cout << "Finished running " << total << " tests." << std::endl;
            std::cout << "  " << oks << " tests passed." << std::endl;
            std::cout << "  " << fails << " tests failed." << std::endl;
            std::cout << "  Longest test took " << max_duration << " ms." << std::endl;
            std::cout << "  Running all took " << int(total_duration+.5) << " seconds." << std::endl;
        }

        return EXIT_SUCCESS;
    }


    return EXIT_FAILURE;
}
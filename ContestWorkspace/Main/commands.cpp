#include "commands.h"
#include "resources.h"
#include "test_discovery.h"
#include "test_execution.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <set>

namespace contest_workspace
{
    bool has_arg(const std::string &arg, const std::vector<std::string> &args)
    {
        return std::find(args.begin()+1, args.end(), arg) != args.end();
    }

    void print_help()
    {
        std::cout << "This program contains tests written using Google Test." << std::endl;
        std::cout << "contest_workspace::tests_path = " << test_path << std::endl;
    }

    void print_test_list()
    {
        const auto tests = discover_tests(test_path);

        for (auto &test_group : tests)
        {
            std::cout << test_group.name << "." << std::endl;
            for (auto &test : test_group.tests)
                std::cout << "  " << test.name << std::endl;
        }
    }

    std::function<bool(const std::string&, const std::string&)> gtest_filter(const std::vector<std::string> &args)
    {
        const std::string prefix = "--gtest_filter=";
        for (auto &arg : args)
        {
            if (arg.substr(0, prefix.size()) != prefix)
                continue;

            std::set<std::string> included_tests;

            auto start = std::find(arg.begin(), arg.end(), '=');

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

    void run_tests(const std::vector<std::string> &args)
    {
        auto tests = discover_tests(test_path);
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
    }
}
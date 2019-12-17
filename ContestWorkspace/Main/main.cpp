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

#include "commands.h"


int main(const int argc, const char *argv[])
{
    using namespace contest_workspace;

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
        run_tests(args);
        return EXIT_SUCCESS;
    }


    return EXIT_FAILURE;
}
#pragma once

#include <vector>
#include <string>

namespace contest_workspace
{
    bool has_arg(const std::string &arg, const std::vector<std::string> &args);

    void print_help();
    void print_test_list();
    void run_tests(const std::vector<std::string> &args);
}
#pragma once

#include "test_discovery.h"

namespace contest_workspace
{
    struct TestResult
    {
        bool result;
        int duration;
    };

    TestResult run_test(const std::string &test_group_name, const Test &test);
}

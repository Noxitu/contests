#pragma once

#include <vector>
#include <string>
#include <experimental/filesystem>
#include <experimental/optional>

namespace fs = std::experimental::filesystem;
using std::experimental::optional;

namespace contest_workspace
{
    struct Test {
        std::string name;
        fs::path input_path;
        optional<fs::path> output_path;
    };

    struct TestGroup {
        std::string name;
        std::vector<Test> tests;
    };

    std::vector<TestGroup> discover_tests(fs::path path);
}

#include "test_discovery.h"
#include <iostream>

std::vector<Test> discover_tests_instances(fs::path root_path)
{
    std::vector<Test> tests;

    for (const auto &entry : fs::directory_iterator(root_path))
    {
        const auto path = entry.path();

        if (path.extension() == ".in")
        {
            std::string name = path.stem().string();
            optional<fs::path> output_path = root_path / (name + ".out");

            if (!fs::exists(*output_path))
                output_path = {};

            tests.push_back({name, path, output_path});
        }
    }

    return tests;
}

std::vector<TestGroup> discover_tests(fs::path root_path)
{
    std::vector<TestGroup> tests;

    {
        auto root_tests = discover_tests_instances(root_path);

        if (!root_tests.empty())
            tests.push_back({"__root__", std::move(root_tests)});
    }

    for (const auto &entry : fs::directory_iterator(root_path))
    {
        const auto path = entry.path();

        if (fs::is_directory(path))
        {
            const std::string group_name = path.filename().string();
            auto sub_tests = discover_tests_instances(path);
            if (!sub_tests.empty())
                tests.push_back({group_name, std::move(sub_tests)});
        }
    }

    return tests;
}
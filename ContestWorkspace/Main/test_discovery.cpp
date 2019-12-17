#include "test_discovery.h"

#include <iostream>

namespace contest_workspace
{
    Test discover_test(const fs::path &path)
    {
        std::string name = path.stem().string();
        optional<fs::path> output_path = path.parent_path() / (name + ".out");

        if (!fs::exists(*output_path))
            output_path = {};

        return {name, path, output_path};
    }

    std::vector<Test> discover_tests_instances(fs::path root_path)
    {
        std::vector<Test> tests;

        if (!fs::is_directory(root_path))
            return tests;

        for (const auto &entry : fs::directory_iterator(root_path))
        {
            const auto path = entry.path();

            if (path.extension() == ".in")
            {
                tests.push_back(discover_test(path));
            }
        }

        return tests;
    }

    std::vector<TestGroup> discover_tests(fs::path root_path)
    {
        std::vector<TestGroup> tests;

        {
            auto root_tests = discover_tests_instances(root_path);

            const fs::path root_test = root_path.string() + ".in";
            std::cout << root_test << std::endl;

            if (fs::is_regular_file(root_test))
                root_tests.insert(root_tests.begin(), discover_test(root_test));

            if (!root_tests.empty())
                tests.push_back({"__root__", std::move(root_tests)});
        }

        if (!fs::is_directory(root_path))
            return tests;

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
}
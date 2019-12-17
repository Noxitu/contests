#pragma once

#include <istream>
#include <ostream>
#include <string>
#include <memory>

namespace contest_workspace
{
    bool compare(std::istream &output, std::istream &truth);
    std::unique_ptr<std::streambuf> online_compare(std::shared_ptr<std::istream> truth, std::ostream &output);
}
#include <istream>
#include <iostream>
#include <string>

bool compare(std::istream &output, std::istream &truth)
{
    while(output || truth)
    {
        std::string lhs, rhs;
        output >> lhs;
        truth >> rhs;

        if (lhs != rhs)
        {
            return false;
        }
    }

    return true;
}
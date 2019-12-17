#include <memory>

namespace contest_workspace
{
    void limit_ram(float mega);
    std::shared_ptr<void> limit_cpu(float sec);
}
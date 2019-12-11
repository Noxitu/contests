#include <sys/resource.h>
#include <stdexcept>

void limit_ram(float mega)
{
    if (mega > 2000)
        throw std::logic_error("Can't limit memory above 2GB");

    rlim_t bytes = static_cast<rlim_t>(mega*1024*1024);

    rlimit rlim;
    rlim.rlim_cur = bytes;
    rlim.rlim_max = bytes;
    setrlimit( RLIMIT_AS, &rlim );
}
#include <bits/stdc++.h>
// #define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define TIME_LIMIT 1
#define MEMORY_LIMIT 128

using namespace std;

void test()
{
    int n;
    cin >> n;

    long long prefix_sum = 0;

    std::vector<long long> last;
    last.reserve(n);
    last.push_back(0);

    for (int i = 0; i < n; ++i)
    {
        int value;
        cin >> value;

        prefix_sum += value;

        if (prefix_sum < 0)
            continue;

        const auto next = std::upper_bound(last.begin(), last.end(), prefix_sum);

        if (next == last.end())
            last.push_back(prefix_sum);
        else
            *next = std::min(*next, prefix_sum);
    }

    const auto it = std::upper_bound(last.begin(), last.end(), prefix_sum);

    const int can_remove = std::distance(last.begin(), it) - 2;
    const int min_cost = n - 1 - can_remove;

    cout << min_cost << '\n';
}


int main()
{
#ifndef CONTEST_WORKSPACE
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
#endif

#ifdef ENDLESS_TESTS
    while(!(cin >> std::ws).eof())
        test();
#else
    int T = 0;

#ifdef MULTIPLE_TESTS
    cin >> T;
#else
    T = 1;
#endif

    while (T --> 0)
        test();
#endif

    return 0;
}
#include <bits/stdc++.h>
// #define MULTIPLE_TESTS
// #define ENDLESS_TESTS

using namespace std;


void test()
{
    int n, k;
    cin >> n >> k;

    int best = std::numeric_limits<int>::max();

    for (int y = 1; y <= n; ++y)
    {
        int total = y;

        for (int x = 1; x <= y; ++x)
        {
            int year;
            cin >> year;

            if (total <= k && year < best)
                best = year;

            total -= x;
            total += (y-x);
        }
    }

    cout << best << '\n';
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
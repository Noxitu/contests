#include <bits/stdc++.h>
// #define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define TIME_LIMIT 0.5
#define MEMORY_LIMIT 256

using namespace std;

void test()
{
    int n;
    cin >> n;

    map<int, int> tasks;

    while (n --> 0)
    {
        int task;
        cin >> std::hex >> task;

        tasks[task] += 1;
    }

    const bool ok = [&]()
    {
        for (int round = 0x10; round <= 0x50; round += 0x10)
            for (int category = 0xa; category <= 0xc; ++category)
            {
                const int count = (round == 0x50 ? 2 : 1);

                if (tasks[round|category] < count)
                    return false;
            }

        return true;
    }();

    cout << (ok ? "TAK" : "NIE") << '\n';
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
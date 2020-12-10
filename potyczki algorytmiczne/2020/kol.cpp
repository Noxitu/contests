#include <bits/stdc++.h>
// #define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define TIME_LIMIT 1
#define MEMORY_LIMIT 128

using namespace std;

void test()
{
    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> changes;
    changes.reserve(2*m);

    while (m --> 0)
    {
        int l, r, k;
        cin >> l >> r >> k;

        changes.emplace_back(l, k);
        changes.emplace_back(r+1, -k);
    }

    sort(changes.begin(), changes.end());

    array<int, 4> status = {};

    int answer = 0;
    constexpr int NOT_OK = -1;
    int ok_since = NOT_OK;

    for (const auto &change : changes)
    {
        if (ok_since != NOT_OK)
        {
            answer += change.first - ok_since;
        }

        const int step = (change.second > 0 ? 1 : -1);
        const int color = abs(change.second);

        status[color] += step;

        const bool ok = (status[1] > 0 && status[2] > 0 && status[3] == 0);

        ok_since = (ok ? change.first : NOT_OK);
    }

    cout << answer << '\n';
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
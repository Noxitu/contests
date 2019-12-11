#include <bits/stdc++.h>
// #define MULTIPLE_TESTS
// #define ENDLESS_TESTS

using namespace std;


struct Score {
    long long previous;
    long long value;
};

void test()
{
    int n;
    long long m;
    cin >> n >> m;

    vector<Score> old_scores = {{-1, 0}};

    while (n --> 0)
    {
        long long a;
        cin >> a;

        vector<Score> new_scores;

        for (const auto &score : old_scores)
        {
            array<bool, 64> visited;
            fill(visited.begin(), visited.end(), false);
            const long long previous = max(0ll, score.previous);

            if (score.previous == -1)
                new_scores.push_back({0, 0});

            for (int j = 0;; ++j)
            {
                const long long bit_mask = (1ll<<j);

                if (previous&bit_mask)
                    continue;

                const long long base = (previous|bit_mask)&~(bit_mask-1);

                if (base > m)
                    break;

                if (visited[__builtin_popcountll(base)])
                    continue;

                for (int i = 0;; ++i)
                {
                    const long long low_mask = (1ll<<i) - 1;
                    const long long b = base | low_mask;

                    if (b > m)
                        break;

                    const int active = __builtin_popcountll(b);

                    bool &was_visited = visited[active];

                    if (was_visited)
                        continue;

                    was_visited = true;
                    new_scores.push_back({b, score.value + a*active});

                    // cout << previous << ' ' << base << ' ' << b << endl;
                }
            }
        }

        std::sort(new_scores.begin(), new_scores.end(), [](auto &lhs, auto &rhs)
        {
            return make_pair(lhs.previous, -lhs.value) < make_pair(rhs.previous, -rhs.value);
        });

        old_scores.clear();
        for (const auto &s : new_scores)
            if (old_scores.empty() || old_scores.back().value < s.value)
                old_scores.push_back(s);

        // cout << "a = " << a << endl;
        // for (auto s : old_scores)
        //     cout << "b = " << s.previous << ";  value = " << s.value << endl;
        // cout << endl;
    }

    cout << old_scores.back().value << '\n';
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
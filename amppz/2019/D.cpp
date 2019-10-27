// solving time = 12m
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

void test()
{
    int n;
    cin >> n;

    vector<pair<int, int>> changes;
    changes.reserve(2*n);

    for (int i = 0; i < n; ++i)
    {
        int r, s;
        cin >> r >> s;
        changes.push_back({i-r, s});
        changes.push_back({i+r+1, -s});
    }

    sort(changes.begin(), changes.end());

    multiset<int> available;

    int answer = 0;

    for (auto it = changes.begin(); it != changes.end();)
    {
        int t = it->first;

        for(; it != changes.end() and it->first == t; ++it)
        {
            int s = it->second;

            if (s < 0)
            {
                auto del = available.find(-s);
                available.erase(del);
            }
            else
                available.insert(s);
        }

        if (available.size() >= 3)
        {
            int current = 0;

            auto best = available.rbegin();
            current += *best;
            current += *++best;
            current += *++best;

            answer = max(answer, current);
        }
    }

    cout << answer << '\n';
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    while (n --> 0)
        test();
}
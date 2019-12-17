#include <bits/stdc++.h>
#define MEMORY_LIMIT 400
#define TIME_LIMIT 5.0

using namespace std;

void test()
{
    int b, n, m;
    cin >> b >> n >> m;

    vector<vector<int>> edges(n);
    
    int lhs=1, rhs=1;

    while (m-->0)
    {
        rhs = (lhs + rhs + b) % n;

        edges[lhs].push_back(rhs);
        if (lhs != rhs)
            edges[rhs].push_back(lhs);

        lhs = (lhs + rhs + b) % n;
    }

    for (auto &current_edges : edges)
    {
        long long sum = 0;
        long long j = 1;

        sort(current_edges.begin(), current_edges.end());

        for (auto l : current_edges)
        {
            sum += j * l;
            j += 1;
        }
        cout << sum << ' ';
    }
    cout << '\n';
}

int main()
{
#ifndef CONTEST_WORKSPACE
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
#endif

    int T = 0;

#if 1
    cin >> T;
#else
    T = 1;
#endif

    while (T --> 0)
        test();

    return 0;
}
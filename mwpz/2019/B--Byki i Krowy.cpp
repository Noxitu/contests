#include <bits/stdc++.h>
#define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define MEMORY_LIMIT 140
#define TIME_LIMIT 5.0
// #define ADDITIONAL_LOGS // enable blocks used only for cerr

template<typename = void> void init() {}

using namespace std;

/******************************************************************************/ 

void test()
{
    map<int, set<int>> E;

    int n, m, k;
    cin >> n >> m >> k;

    vector<vector<char>> edges_in_k(k, vector<char>(k, false));

    while (m --> 0)
    {
        int a, b;
        cin >> a >> b;

        a -= 1;
        b -= 1;

        if (b < k)
        {
            edges_in_k[a][b] = edges_in_k[b][a] = true;
        }
        else if (static_cast<int>(E[a].size()) <= k)
        {
            E[a].insert(b);
            E[b].insert(a);
        }
    }

    for (int a = 0; a < k; ++a)
        for (int b = a+1; b < k; ++b)
            if (edges_in_k[a][b] == false)
            {
                E[a].insert(b);
                E[b].insert(a);
            }

    set<int> Vc;

    while (true)
    {
        vector<int> remove;

        for (auto &e : E)
        {
            const auto v = e.first;

            if (static_cast<int>(E[v].size()) >= k)
            {
                Vc.insert(v);
                remove.push_back(v);
            }
        }

        for (auto r : remove)
        {
            for (auto u : E[r])
            {
                E[u].erase(r);
            }

            E.erase(r);
        }

        if (remove.empty())
            break;
    }

    auto solve = [&](auto solve) -> bool
    {
        if (static_cast<int>(Vc.size()) >= k)
            return false;

        if (E.empty())
            return true;

        bool has_neighbor_with_1 = false;
        int v = -1;
        size_t best_size = 0;

        vector<int> empty;

        for (auto &e : E)
            if (e.second.size() == 0)
                empty.push_back(e.first);

        for (auto v : empty)
            E.erase(v);

        for (auto &e : E)
        {
            if (e.second.size() == 1)
            {
                v = *e.second.begin();
                has_neighbor_with_1 = true;
                break;
            }
            else if (e.second.size() >= best_size)
            {
                v = e.first;
                best_size = e.second.size();
            }
        }

        auto remove = [&](auto v)
        {
            auto archive = move(E[v]);
            
            E.erase(v);

            for (auto u : archive)
            {
                E[u].erase(v);
            }

            return archive;
        };

        auto restore = [&](auto v, auto archive)
        {
            for (auto u : archive)
            {
                E[u].insert(v);
            }

            E[v] = move(archive);
        };

        Vc.insert(v);
        auto archive_v = remove(v);
        
        if (solve(solve))
            return true;

        Vc.erase(v);

        if (!has_neighbor_with_1)
        {
            stack<pair<int, set<int>>> archives_u;

            for (auto u : archive_v)
            {
                auto archive_u = remove(u);
                archives_u.push({u, move(archive_u)});
                Vc.insert(u);
            }

            if (solve(solve))
                return true;

            while(!archives_u.empty())
            {
                auto archive_u = move(archives_u.top());
                archives_u.pop();
                auto u = archive_u.first;
                restore(u, move(archive_u.second));
                Vc.erase(u);
            }
        }

        restore(v, archive_v);

        for (auto v : empty)
            E[v];

        return false;
    };

    const bool has_small_coverage = solve(solve);

    if (has_small_coverage)
    {
        set<int> solution;
        for (int i = 0; i < k; ++i)
            if (Vc.count(i) == 0)
                solution.insert(i);

        cout << "NO\n";
        cout << solution.size() << '\n';
        for (auto v : solution)
            cout << v+1 << ' ';
        cout << '\n';
    }
    else
    {
        cout << "YES\n";
    }
    
}

/******************************************************************************/

int main()
{
    init();

#if !defined(CONTEST_WORKSPACE)
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
#endif

#if defined(MULTIPLE_TESTS)
    int T = 0;
    cin >> T;

    while (T --> 0)
        test();

#elif defined(ENDLESS_TESTS)
    while(!(cin >> std::ws).eof())
        test();

#else
    test();
#endif

    return EXIT_SUCCESS;
}

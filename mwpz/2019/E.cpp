#include <bits/stdc++.h>
#define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define MEMORY_LIMIT 256
#define TIME_LIMIT 15.0
// #define ADDITIONAL_LOGS // enable blocks used only for cerr

using namespace std;


void test()
{
    int n;
    cin >> n;

    vector<array<int, 3>> vertices(n);

    for (auto &v : vertices)
    {
        for (auto &e : v)
        {
            cin >> e;
            e -= 1;
        }
    }

    vector<char> assignment(n);

    for (auto &a : assignment)
        a = rand()%2;

    unordered_set<int> wrong_vertices;

    auto status = [&](int a)
    {
        int same_assignment = 0;

        for (int b : vertices[a])
            if (assignment[a] == assignment[b])
                same_assignment += 1;
        
        return same_assignment < 2;
    };

    for (int i = 0; i < n; ++i)
        if (status(i) == false)
            wrong_vertices.insert(i);

    while (!wrong_vertices.empty())
    {
        const int a = *wrong_vertices.begin();
        assignment[a] ^= 1;

        wrong_vertices.erase(a);

        for (int b : vertices[a])
            if (status(b))
                wrong_vertices.erase(b);
            else
                wrong_vertices.insert(b);
    }

    array<int, 2> sizes = {0, 0};

    for (int i = 0; i < n; ++i)
        sizes[assignment[i]] += 1;

    for (int group : {0, 1})
    {
        cout << sizes[group] << '\n';
        for (int i = 0; i < n; ++i)
            if (assignment[i] == group)
                cout << i+1 << ' ';
        cout << '\n';
    }
}

/******************************************************************************/

int main()
{
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

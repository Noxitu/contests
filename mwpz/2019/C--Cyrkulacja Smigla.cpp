#include <bits/stdc++.h>
#define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define MEMORY_LIMIT 256
#define TIME_LIMIT 5.0
// #define ADDITIONAL_LOGS // enable blocks used only for cerr

template<typename = void> void init() {}

using namespace std;

/******************************************************************************/

struct Point
{
    int x, y;
};

int orientation(Point first, Point mid, Point last) 
{ 
    const long long val = 1LL * (mid.y - first.y) * (last.x - mid.x) - 
                          1LL * (mid.x - first.x) * (last.y - mid.y); 
  
    if (val == 0) return 0;
    return val > 0 ? 1 : -1;
} 

void test()
{
    int n;
    cin >> n;

    vector<Point> points(n);
    for (auto &p : points)
        cin >> p.x >> p.y;

    map<pair<int, int>, vector<int>> answers;

    auto solve = [&answers](int origin_index, vector<Point> points)
    {
        const auto origin = points[origin_index];
        points.erase(points.begin()+origin_index);
        const int n = points.size();

        sort(points.begin(), points.end(), [&](auto &lhs, auto &rhs)
        {
            const bool lhs_in_first_half = lhs.x > origin.x;
            const bool rhs_in_first_half = rhs.x > origin.x;

            if (lhs_in_first_half && !rhs_in_first_half) return true;
            if (!lhs_in_first_half && rhs_in_first_half) return false;
            
            return orientation(lhs, origin, rhs) == -1;
        });
    
        int mid = 0;
        while(mid < n && points[mid].x > origin.x) ++mid;

        map<pair<int, int>, int> answers_for_point;

        int x0 = 0;
        int y0 = mid;
        while (x0 != mid || y0 != n)
        {
            const bool use_left = [&]()
            {
                if (y0 == n) return true;
                if (x0 == mid) return false;
                return orientation(points[x0], origin, points[y0]) == 1;
            }();

            if (use_left)
                x0 += 1;
            else
                y0 += 1;
            
            const int x = y0-x0;
            const int y = n-x;

            answers_for_point[{x, y}] += 1;
            answers_for_point[{y, x}] += 1;
        }

        for (auto &ans : answers_for_point)
        {
            answers[ans.first].push_back(ans.second);
        }
    };

    for (int origin = 0; origin < n; ++origin)
    {
        solve(origin, points);
    }

    vector<vector<int>> sorted_answers;

    for (auto &ans : answers)
    {
        auto vec = move(ans.second);

        vec.resize(n, 0);

        sort(vec.begin(), vec.end());
        sorted_answers.push_back(move(vec));
    }

    for (int i = n - 1; i >= 0; --i)
    {
        auto key = [i](auto &vec)
        {
            return vec[i];
        };

        stable_sort(sorted_answers.begin(), sorted_answers.end(), [&](auto &lhs, auto &rhs)
        {
            return key(lhs) < key(rhs);
        });
    }

    vector<int> common_prefix = {0};

    for (int i = 1; i < n; ++i)
    {
        const auto &prev = sorted_answers[i-1];
        const auto &next = sorted_answers[i];

        int ret = 0;

        while (ret < n && prev[ret] == next[ret])
            ret += 1;

        common_prefix.push_back(ret);
    }

    // for (auto &vec : sorted_answers)
    // {
    //     for (auto v : vec)
    //         cout << v << ' ';
    //     cout << endl;
    // }
    // cout << endl;

    int m;
    cin >> m;

    while (m --> 0)
    {
        int k;
        cin >> k;
        vector<int> query(k);
        for (auto &a : query)
            cin >> a;

        query.resize(n, 0);
        sort(query.begin(), query.end());

        const bool answer = [&]()
        {
            int matched_prefix = 0;

            for (int i = 0; i < n; ++i)
            {
                if (matched_prefix > common_prefix[i])
                    return false;

                const auto &vec = sorted_answers[i];
                while (matched_prefix < n && query[matched_prefix] == vec[matched_prefix])
                    matched_prefix += 1;

                if (matched_prefix == n)
                    return true;
            }

            return false;
        }();

        cout << (answer ? "YES\n" : "NO\n");
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

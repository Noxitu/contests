#include <bits/stdc++.h>
#define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define MEMORY_LIMIT 512

using namespace std;

auto read = [](auto &vec)
{
    int n;
    cin >> n;
    vec.reserve((n+1)/2);

    for (int i = 0; i < n; ++i)
    {
        int val;
        cin >> val;
        if (val == 1)
            vec.push_back(i);
    }
};

struct SingleResult
{
    int single_pos = -1;
    int first_pos = 0;
    int count = 0;
};

SingleResult mul(int a, int b)
{
    if (a > b) swap(a, b);

    SingleResult ret;
    ret.count = a / 2 + 1;

    const bool even = (a%2 == 0);
    ret.first_pos = b-a;

    if (!even)
    {
        const bool equal = (a==b);
        ret.first_pos += 2;
        ret.single_pos = ret.first_pos - (equal ?  2 : 3);
    }

    return ret;
}

void test()
{
    vector<int> x;
    vector<int> y;
    read(x);
    read(y);

    // if (1LL * x.size() * y.size() > 1000000000)
    // {
    //     cout << "Too much data\n";
    //     return;
    // }

    vector<int> result;

    auto at = [&](int idx) -> int&
    {
        if (idx >= static_cast<int>(result.size()))
            result.resize(idx+1, 0);
        return result[idx];
    };

    auto fix = [&result, &at]()
    {
        int i = result.size()-1;

        while (i >= 0)
        {
            // cout << "fix @ " << i << endl;

            // for (int j = i - 3; j < i + 3 && j < result.size(); ++j)
            //     cout << result[j] << ' ';
            // cout << endl;

            if (result[i] == 0)
            {
                i -= 1;
                continue;
            }

            if (i == 0)
            {
                if (result[0] == 1)
                    break;

                at(1) += result[0]/2;
                result[0] %= 2;
                i = 1;
                continue;
            }

            if (at(i+1) > 0)
            {
                i += 1;
                continue;
            }

            if (result[i] == 1 && result[i-1] == 0)
            {
                i -= 1;
                continue;
            }

            if (result[i] > 0 && result[i-1] > 0)
            {
                const int step = min(result[i], result[i-1]);
                result[i-1] -= step;
                result[i] -= step;
                at(i+1) += step;
                i += 1;
                continue;
            }

            assert(result[i] > 0);

            if (i == 1)
            {
                const int step = max(1, result[i]/3);
                result[1] -= step;
                result[0] += 2*step;
                continue;
            }

            const int step = max(1, result[i]/2);
            result[i] -= step;
            result[i-1] += step;
            result[i-2] += step;
            continue;
        }

        while(result.back() == 0)
            result.pop_back();
    };

    auto print = [&]()
    {
        cout << result.size();
        for (auto v : result)
            cout << ' ' << v;
        cout << '\n';
    };

    vector<int> changes;

    auto changes_at = [&](int idx) -> int&
    {
        if (idx >= static_cast<int>(changes.size()))
            changes.resize(idx+1, 0);
        return changes[idx];
    };

    for (auto a : x)
        for (auto b : y)
        {
            auto tmp = mul(a, b);

            if (tmp.single_pos != -1)
                at(tmp.single_pos) += 1;

            changes_at(tmp.first_pos) += 1;
            changes_at(tmp.first_pos + 4*tmp.count) -= 1;
        }

    {
        array<int, 4> state = {0, 0, 0, 0};
        result.resize(changes.size(), 0);

        for (auto i = 0u; i < changes.size(); ++i)
        {
            auto &s = state[i%4];
            s += changes[i];
            result[i] += s;
        }
    }

    fix();
    print();
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

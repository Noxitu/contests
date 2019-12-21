#include <bits/stdc++.h>
#define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define MEMORY_LIMIT 256
#define TIME_LIMIT 5.0
// #define ADDITIONAL_LOGS // enable blocks used only for cerr

using namespace std;

/******************************************************************************/

constexpr int MOD = 1'000'000'007;

unordered_map<int, int> F;

int compute(int x)
{
    auto it = F.find(x);

    if (it != F.end())
        return it->second;

    long long answer = 1;

    auto handle = [&](int k)
    {
        if (k == 1)
            return;

        const int param = x/k;
        const int value = compute(param);
        const int low = x/(param+1)+1;
        const int high = k;

        assert(x/(low-1) != param);
        assert(x/low == param);
        assert(x/high == param);
        assert(x/(high+1) != param);

        const int count = high-low+1;
        answer += 1LL * count * value % MOD;
    };

    for (int k = 1; k*k <= x; ++k)
    {
        const int other = x/k;

        handle(k);

        if (other != k)
            handle(other);
    }

    const int ret = answer % MOD;
    F[x] = ret;
    return ret;
}

int solve(int l, int r)
{
    long long answer = 0;

    auto handle = [&](int k)
    {
        const int high = k;

        if (high < l)
            return;

        const int param = r/k;
        const int value = compute(param);
        const int low = max(l, r/(param+1)+1);

        // assert(x/(low-1) != param);  // not true anymore due to low = max(...)
        assert(r/low == param);
        assert(r/high == param);
        assert(r/(high+1) != param);

        const int count = high-low+1;
        answer += 1LL * count * value % MOD;
    };

    for (int k = 1; k*k <= r; ++k)
    {
        const int other = r/k;

        handle(k);

        if (other != k)
            handle(other);
    }

    return answer % MOD;
}

void test()
{
    int low, high;
    cin >> low >> high;

    F[0] = 0;
    F[1] = 1;

    cout << solve(low, high) << '\n';
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

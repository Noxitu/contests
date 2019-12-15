#include <bits/stdc++.h>
// #define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define MEMORY_LIMIT 312
#define TIME_LIMIT 2.0
// #define ADDITIONAL_LOGS // enable blocks used only for cerr

using namespace std;

constexpr int MOD = 1'000'000'000+7;

inline int fast_pow(int a, int power)
{
    int result = 1;
    while (power > 0)
    {
        if (power%2 == 1)
            result = 1LL * result * a % MOD;

        power /= 2;
        a = 1LL * a * a % MOD;
    }

    return result;
}

// Małe Twierdzenie Fermata
inline int mod_inverse(int a)
{
    return fast_pow(a, MOD-2);
}

vector<int> inverses(int n)
{
    vector<int> ret(n+1);

    for (int i = 0; i <= n; ++i)
        ret[i] = mod_inverse(i);

    return ret;
}

vector<int> pascal_row(int n, const vector<int> &inverses)
{
    vector<int> row(n+1);
    row[0] = 1;

    for (int k = 0; k < n; ++k)
    {
        row[k+1] = 1LL * row[k] * (n-k) % MOD * inverses[k+1] % MOD;
    }

    return row;
}

vector<int> sufix_sums(const vector<int> &row)
{
    vector<int> sums(row.size());

    int current = 0;

    for (int i = row.size()-1; i >= 0; --i)
    {
        current = (current + row[i]) % MOD;
        sums[i] = current;
    }

    return sums;
}

void solve_optimize_123(int r1, int r2, int r3, int c12, int c13, int c23, int c123)
{
    const int n = c12+c13+c23+c123;
    const int max_n = max({c23, c13, c12, c123});

    const vector<int> inverses = ::inverses(max_n);

    const auto syms23 = pascal_row(c23, inverses);
    const auto syms13 = pascal_row(c13, inverses);
    const auto syms12 = pascal_row(c12, inverses);
    const auto syms123 = pascal_row(c123, inverses);
    const auto sufixs123 = sufix_sums(syms123);

    int answer = fast_pow(2, n);

    const int base1 = r1 + 1;
    const int base2 = r2 - c23 - c13 + 1;
    const int base3 = r3 - c23 - c12 + 1;

    unordered_map<int, vector<long long>> req_method1;
    unordered_map<int, vector<long long>> req_method2;

    for (int i23 = 0; i23 <= c23; ++i23)
    {
        const int sym23 = syms23[i23];

        for (int i13 = 0; i13 <= c13; ++i13)
        {
            const int prefix = 1LL * sym23 * syms13[i13] % MOD;
            const int req1 = max({base1-i23-i13, base2+i23+i13});
            const int req2 = base3+i23-i13;

            const int min12 = max(0, req1-c123);
            const int max12 = min(c12, c123-req2);
            const int swap = (req1-req2+1)/2;

            const int range_end1 = min(max12, swap-1);
            const int range_begin2 = max(min12, swap);

            if (min12 <= range_end1)
            {
                auto &vec = req_method1[req1];
                vec.resize(c12+2);
                vec[min12] += prefix;
                vec[range_end1+1] -= prefix;
            }

            if (range_begin2 <= max12)
            {
                auto &vec = req_method2[req2];
                vec.resize(c12+2);
                vec[range_begin2] += prefix;
                vec[max12+1] -= prefix;
            }
        }
    }

    for (const auto &elem : req_method1)
    {
        const int req1 = elem.first;
        const auto &vec = elem.second;

        long long prefix = 0;

        for (int i12 = 0; i12 <= c12; ++i12)
        {
            prefix += vec[i12];
            const int req = max(0, req1-i12);

            if(req > c123)
                continue;

            const int sufix = 1LL * syms12[i12] * sufixs123[req] % MOD;
            const int diff = prefix % MOD * sufix % MOD;
            answer = (answer - diff + MOD) % MOD;
        }
    }

    for (const auto &elem : req_method2)
    {
        const int req2 = elem.first;
        const auto &vec = elem.second;

        long long prefix = 0;

        for (int i12 = 0; i12 <= c12; ++i12)
        {
            prefix += vec[i12];
            const int req = max(0, req2+i12);

            if(req > c123)
                continue;

            const int sufix = 1LL * syms12[i12] * sufixs123[req] % MOD;
            const int diff = prefix % MOD * sufix % MOD;
            answer = (answer - diff + MOD) % MOD;
        }
    }

    cout << answer << '\n';
}

void test()
{
    int n;
    cin >> n;

    int r1, r2, r3;
    string bits1, bits2, bits3;

    cin >> r1 >> bits1;
    cin >> r2 >> bits2;
    cin >> r3 >> bits3;

    int c12=0, c13=0, c23=0, c123=0;

    for (int i = 0; i < n; ++i)
    {
        const bool b12 = (bits1[i] == bits2[i]);
        const bool b13 = (bits1[i] == bits3[i]);

        if (b12 && b13) ++c123;
        else if (b12) ++c12;
        else if (b13) ++c13;
        else ++c23;
    }

    // cerr << c23 << ' ' << c13 << ' ' << c12 << ' ' << c123 << endl;

    if (c13 > c12)
    {
        swap(c13, c12);
        swap(r3, r2);
    }

    if (c23 > c12)
    {
        swap(c23, c12);
        swap(r3, r1);
    }

    solve_optimize_123(r1, r2, r3, c12, c13, c23, c123);          
}

/******************************************************************************/

int main()
{
#ifdef CONTEST_WORKSPACE
    #ifdef MEMORY_LIMIT
        void limit_ram(float);
        limit_ram(MEMORY_LIMIT);
    #endif
    #ifdef TIME_LIMIT
        void limit_cpu(float);
        limit_cpu(TIME_LIMIT);
    #endif
#endif

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

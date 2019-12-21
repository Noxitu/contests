#include <bits/stdc++.h>
#define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define MEMORY_LIMIT 256
#define TIME_LIMIT 5.0
// #define ADDITIONAL_LOGS // enable blocks used only for cerr

template<typename = void> void init() {}

using namespace std;

/******************************************************************************/

constexpr int MAX_DIGITS = 18;
constexpr int ANY_N = -1;

vector<map<long long, long long>> counts;

void init()
{
    counts = vector<map<long long, long long>>(MAX_DIGITS+1);

    counts[0][1] = 1;

    for (int digits = 1; digits <= MAX_DIGITS; ++digits)
    {
        auto &new_counts = counts[digits];

        for (const auto &old : counts[digits-1])
        {
            for (int new_digit = 0; new_digit < 10; ++new_digit)
            {
                const long long new_product = old.first*new_digit;
                new_counts[new_product] += old.second;
            }

        }
    }

    counts[0][ANY_N] = 1;

    for (int digits = 1; digits <= MAX_DIGITS; ++digits)
        counts[digits][ANY_N] = 10*counts[digits-1][ANY_N];
}

void solve(long long n, long long k)
{
    int answer_digits = 1;
    while (true)
    {
        if (answer_digits >= static_cast<int>(counts.size()))
        {
            cout << "NO\n";
            return;
        }

        const auto &current_counts = counts[answer_digits];
        const auto it = current_counts.find(n);

        const long long current_count = it != current_counts.end() ? it->second : 0;

        if (current_count >= k)
            break;

        k -= current_count;

        answer_digits += 1;
    }

    // cerr << "answer has " << answer_digits << " digits" << endl;

    while (answer_digits > 0)
    {
        // cerr << "n=" << n << "    k=" << k << endl;
        for (int digit = 1; digit < 10; ++digit)
        {
            if (n % digit != 0)
                continue;

            const long long new_n = n / digit;
            const long long current_count = counts[answer_digits-1][new_n];

            // cerr << "  " << current_count << " possibilites with digit " << digit << endl;

            if (current_count >= k)
            {
                n = new_n;
                cout << digit;
                break;
            }

            k -= current_count;
        }

        answer_digits -= 1;
    }

    cout << '\n';
}

void solve0(long long k)
{
    // cerr << "n=0  k=" << k << endl;
    if (k == 1)
    {
        cout << "0\n";
        return;
    }
    k -= 1;

    long long n = 0;

    int answer_digits = 1;

    while (true)
    {
        if (answer_digits-1 >= static_cast<int>(counts.size()))
        {
            cout << "NO\n";
            return;
        }

        const auto &current_counts = counts[answer_digits-1];
        const auto it = current_counts.find(n);

        const long long current_count = it != current_counts.end() ? it->second : 0;

        if (9*current_count >= k)
        {
            const int first_digit = (k-1) / current_count + 1;
            k -= (first_digit-1) * current_count;

            if (answer_digits > MAX_DIGITS)
            {
                if (k == 1)
                    cout << "1000000000000000000\n";
                else
                    cout << "NO\n";
                return;
            }

            cout << first_digit;
            break;
        }

        k -= 9*current_count;

        answer_digits += 1;
    }

    // cerr << "answer_digits=" << answer_digits << endl;
    answer_digits -= 1;

    while (answer_digits > 0)
    {
        // cerr << "n=" << n << "    k=" << k << endl;
        for (int digit = 0; digit < 10; ++digit)
        {
            const long long new_n = digit == 0 ? ANY_N : n;
            const long long current_count = counts[answer_digits-1][new_n];

            // cerr << "  " << current_count << " possibilites with digit " << digit << "  counts[" << answer_digits-1 << "][" << new_n << "]" << endl;

            if (current_count >= k)
            {
                n = new_n;
                cout << digit;
                break;
            }

            k -= current_count;
        }

        answer_digits -= 1;
    }

    cout << '\n';
}

void test()
{

    long long n, k;
    cin >> n >> k;

    if (n != 0)
        solve(n, k);
    else
        solve0(k);
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

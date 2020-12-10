#include <bits/stdc++.h>
// #define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define TIME_LIMIT 0.3
#define MEMORY_LIMIT 128

using namespace std;

pair<string, string> get_data(int n)
{
    string input;
    cin >> input;

    string even, odd;
    even.reserve((n+1)/2);
    odd.reserve(n/2);

    for (int i = 0; i < n; i += 2)
        even.push_back(input[i]);

    for (int i = 1; i < n; i += 2)
        odd.push_back(input[i]);

    sort(even.begin(), even.end());
    sort(odd.begin(), odd.end());

    return {even, odd};
}

void test()
{
    int n;
    cin >> n;

    const auto [even1, odd1] = get_data(n);
    const auto [even2, odd2] = get_data(n);

    const bool ok = (even1 == even2 && odd1 == odd2);

    cout << (ok ? "TAK\n" : "NIE\n");
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
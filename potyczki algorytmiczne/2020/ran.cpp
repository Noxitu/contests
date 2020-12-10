#include <bits/stdc++.h>
// #define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define TIME_LIMIT 2
#define MEMORY_LIMIT 128

using namespace std;

void test()
{
    int n;
    cin >> n;
    vector<int> input(n);

    for (auto &value : input)
        cin >> value;

    vector<int> output(n);

    for (int index = 0; index < n; ++index)
    {
        const int prev = (index == 0 ? 0 : input[index-1]);
        output[index] = input[index] - prev;

        int partial_sum = 0;
        for (int offset = 0; offset <= index; ++offset)
        {
            partial_sum += output[index-offset];

            if (partial_sum > input[offset])
            {
                cout << "NIE\n";
                return;
            }
        }
    }

    cout << "TAK\n";
    cout << n << '\n';

    for (auto value : output)
        cout << value << ' ';
    cout << '\n';
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
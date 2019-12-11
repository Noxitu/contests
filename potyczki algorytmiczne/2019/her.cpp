#include <bits/stdc++.h>
#define MULTIPLE_TESTS
// #define ENDLESS_TESTS

using namespace std;

struct Tea {
    long long amount;
    long long temperature;
};

auto comp = [](auto &lhs, auto &rhs)
{
    return lhs.temperature < rhs.temperature;
};

void test()
{
    int n;
    cin >> n;

    vector<Tea> input(n);
    vector<Tea> output(n);

    long long input_heat = 0;
    long long output_heat = 0;

    for (int i = 0; i < n; ++i)
    {
        int amount, in, out;
        cin >> amount >> in >> out;

        input[i] = {amount, in};
        output[i] = {amount, out};

        input_heat += 1LL * amount * in;
        output_heat += 1LL * amount * out;
    }

    if (input_heat != output_heat)
    {
        cout << "NIE\n";
        return;
    }

    sort(input.begin(), input.end(), comp);
    sort(output.begin(), output.end(), comp);

    long long heat = 0;
    long long amount = 0;

    auto it = input.begin();
    const auto end = input.end();

    const auto advance_it = [&](long long required_amount)
    {
        long long missing_amount = required_amount-amount;
        int transfered_amount = min(missing_amount, it->amount);
        amount += transfered_amount;
        it->amount -= transfered_amount;

        heat += transfered_amount * it->temperature;

        if (it->amount == 0)
            ++it;
    };

    for (auto &out : output)
    {
        while (it != end && amount < out.amount)
            advance_it(out.amount);

        heat -= out.amount * out.temperature;

        if (heat > 0)
        {
            cout << "NIE\n";
            return;
        }

        amount = 0;
    }

    cout << "TAK\n";
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
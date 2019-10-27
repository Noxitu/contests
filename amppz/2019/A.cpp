// solving time = 10m
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

void test()
{
    int n, k;
    cin >> n >> k;

    multiset<int> sizes;

    for (int i = 0; i < n; ++i)
    {
        int s;
        cin >> s;
        sizes.insert(s);
    }

    long long available = k;
    int mobilizations = 0;

    while (true)
    {
        // cerr << "available = " << available << endl;
        auto non_capturable = sizes.upper_bound(available);

        if (non_capturable == sizes.end())
        {
            cout << mobilizations << '\n';
            return;
        }

        // cerr << "cant capture = " << *non_capturable << endl;

        if (non_capturable == sizes.begin())
        {
            cout << "-1\n";
            return;
        }

        auto capturable = prev(non_capturable);

        // cerr << "can capture = " << *capturable << endl;

        mobilizations += 1;
        available += *capturable;
        sizes.erase(capturable);
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    while (n --> 0)
        test();
}
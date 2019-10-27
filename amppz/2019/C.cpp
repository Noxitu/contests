// solving duration = ~10m
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void test()
{
    int n;
    cin >> n;
    
    vector<int> lengths(n);
    for (auto &l : lengths)
        cin >> l;

    sort(lengths.begin(), lengths.end());

    long long length_before = 0;
    long long answer = 0;
    for (int i = 0; i < n; ++i)
    {
        const auto l = lengths[i];

        if (i >= 2 and l < length_before)
        {
            answer = length_before + l;
        }

        length_before += l;
    }

    cout << answer << '\n';
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
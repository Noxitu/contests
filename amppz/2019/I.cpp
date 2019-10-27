// solving time = 45m
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

constexpr int NO_ASSIGNMENT = 10'000'000;
constexpr int OPPOSITE = 20'000'000;

void test()
{
    int n;
    cin >> n;

    vector<int> radius(n);
    for (auto &r : radius)
        cin >> r;

    vector<vector<int>> assignment(n);
    int free = 0;

    int farthest_palindrome_position = -1;
    int farthest_palindrome_radius = 0;

    for (int i = 0; i < n; ++i)
    {
        const int palindrome_end = farthest_palindrome_position + farthest_palindrome_radius;
        const int mirror = farthest_palindrome_position - (i - farthest_palindrome_position);

        if (i+radius[i] > farthest_palindrome_position + farthest_palindrome_radius)
        {
            farthest_palindrome_position = i;
            farthest_palindrome_radius = radius[i];
        }

        const bool is_known = (i <= palindrome_end);

        if (is_known)
        {
            //cerr << "#" << i << " = known\n";
            assignment[i].push_back(mirror);
        }

        const int first_after = i+radius[i]+1;
        const int first_before = i-radius[i]-1;

        if (first_before >= 0 and first_after < n)
        {
            //cerr << "#" << first_after << " = oppsite\n";
            assignment[first_after].push_back(OPPOSITE+first_before);
        }

        if (assignment[i].empty())
        {
            //cerr << "#" << i << " = free\n";
            free += 1;
        }
    }

    int solutions = (1<<free);
    //cout << solutions << '\n';

    vector<char> values(n);
    for (int mask = 0; mask < solutions; ++mask)
    {
        int current_mask = (solutions>>1);

        for (int i = 0; i < n; ++i)
        {
            if (assignment[i].empty())
            {
                values[i] = ((mask & current_mask) ? 1 : 0);
                current_mask >>= 1;

                //cerr << "#" << i << " = " << static_cast<int>(values[i]) << '\n';

                continue;
            }

            int value = 0;
            int count = 0;

            for (auto a : assignment[i])
            {
                if (a >= OPPOSITE)
                {
                    //cerr << "#" << i << " = OPPOSITE[" << a-OPPOSITE << "] = " << static_cast<int>(1 - values[a-OPPOSITE]) << '\n';
                    value += 1 - values[a-OPPOSITE];
                }
                else
                {
                    //cerr << "#" << i << " = SAME[" << a << "] = " << static_cast<int>(values[a]) << '\n';
                    value += values[a];
                }

                count += 1;

                if (mask != 0)
                {
                    break;
                }
            }

            if (mask == 0)
            {
                if (value != count and value != 0)
                    solutions = 0;

                value /= count;
            }

            values[i] = value;
        }

        if (mask == 0)
        {
            cout << solutions << '\n';

            if (solutions == 0)
                return;
        }

        for (auto v : values)
            cout << static_cast<int>(v);
        cout << '\n';
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
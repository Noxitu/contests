// solving time = 14m
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

struct Answers
{
    long long last_eaten = 0;
    long long single_skip = 0;
    long long double_skip = 0;
};

void test()
{
    int n;
    cin >> n;

    Answers answer;

    int o_prev = 0, o_2prev = 0;

    for (int i = 0; i < n; ++i)
    {
        int o;
        cin >> o;

        Answers next;

        //cerr << "{ " << answer.last_eaten << ", " << answer.single_skip << ", " << answer.double_skip << " }  [" << o_2prev << ", " << o_prev << ", " << o << "]\n";

        next.last_eaten = max(answer.single_skip + o, 
                              answer.double_skip + o + min(o_prev, o_2prev));
        next.single_skip = answer.last_eaten;
        next.double_skip = answer.single_skip;

        answer = next;

        o_2prev = o_prev;
        o_prev = o;

    }

    cout << max({answer.last_eaten, answer.single_skip}) << '\n';
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

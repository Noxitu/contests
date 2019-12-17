#include <bits/stdc++.h>
#define MEMORY_LIMIT 400
#define TIME_LIMIT 5.0

using namespace std;

struct Score
{
    int first;
    int second;
    int tie_break_result;
};

void test()
{
    while(true)
    {
        string input;
        cin >> input;
        if (input.empty())
            break;

        const char *ptr = input.c_str();
        int first_serve = 0;

        auto play_gem = [&](bool tie_break = false) -> pair<int, int>
        {
            int serve = first_serve;
            //if (!tie_break)
                first_serve = 1-first_serve;

            array<int, 2> gem_score = {0, 0};

            const int win_score = tie_break ? 7 : 4;
            bool odd_serve = true;

            while(true)
            {
                if (gem_score[0] >= max(win_score, gem_score[1]+2))
                {
                    // cout << "0 won gem " << gem_score[0] << '-' << gem_score[1] << endl;
                    return {0, gem_score[1]};
                }

                if (gem_score[1] >= max(win_score, gem_score[0]+2))
                {
                    // cout << "1 won gem " << gem_score[0] << '-' << gem_score[1] << endl;
                    return {1, gem_score[0]};
                }

                switch(*(ptr++))
                {
                    case 'A':
                    case 'S':
                        gem_score[serve] += 1;
                        break;
                    case 'R':
                    case 'D':
                        gem_score[1-serve] += 1;
                        break;
                }


                if (odd_serve && tie_break)
                    serve = 1-serve;

                odd_serve = !odd_serve;
            }
        };

        auto play_set = [&]() -> pair<int, Score>
        {
            array<int, 2> set_score = {0, 0};

            while (true)
            {
                if (set_score[0] >= max(6, set_score[1]+2))
                {
                    // cout << "0 won set " << set_score[0] << '-' << set_score[1] << endl;
                    return {0, {set_score[0], set_score[1], -1}};
                }

                if (set_score[1] >= max(6, set_score[0]+2))
                {
                    // cout << "1 won set " << set_score[0] << '-' << set_score[1] << endl;
                    return {1, {set_score[0], set_score[1], -1}};
                }

                if (set_score[0] == 6 && set_score[1] == 6)
                {
                    auto result = play_gem(true);
                    set_score[result.first] += 1;
                    // cout << result.first << " won set " << set_score[0] << '-' << set_score[1] << '(' << result.second << ')' << endl;
                    return {result.first, {set_score[0], set_score[1], result.second}};
                }

                int gem_winner = play_gem().first;
                set_score[gem_winner] += 1;
            }
        };

        auto play_all = [&]() 
        {
            array<int, 2> game_score = {0, 0};
            vector<Score> scores;

            while (*ptr)
            {
                auto result = play_set();
                game_score[result.first] += 1;
                scores.push_back(result.second);
            }

            auto first = &Score::first;
            auto second = &Score::second;

            if (game_score[1] > game_score[0])
                swap(first, second);

            for (auto s : scores)
            {
                cout << s.*first << '-' << s.*second;

                if (s.tie_break_result != -1)
                    cout << '(' << s.tie_break_result << ')';
                cout << ' ';
            }

            cout << '\n';
        };

        play_all();
    }
}

int main()
{
#ifndef CONTEST_WORKSPACE
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
#endif

    int T = 0;

#if 0
    cin >> T;
#else
    T = 1;
#endif

    while (T --> 0)
        test();

    return 0;
}
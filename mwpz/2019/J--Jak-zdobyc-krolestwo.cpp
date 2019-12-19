#include <bits/stdc++.h>
// #define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define MEMORY_LIMIT 256
#define TIME_LIMIT 5.0
// #define ADDITIONAL_LOGS // enable blocks used only for cerr

using namespace std;

/******************************************************************************/

void test()
{
    int rows, cols;
    cin >> rows >> cols;

    vector<vector<short>> map(rows, vector<short>(cols));
    for (auto &row : map)
        for (auto &cell : row)
            cin >> cell;

    stringstream answer;

    auto draw = [&]()
    {
        for (auto &row : map)
        {
            for (auto &cell : row)
                answer << (cell == 1 ? '#' : '.');
            answer << endl;
        }
        answer << endl;
    };

    // draw();

    auto swap_row = [&](int y)
    {
        for (int x1 = cols/2; x1 < cols; ++x1)
        {
            const int x2 = cols-1-x1;
            tie(map[y][x1], map[y][x2]) = make_tuple(map[y][x2]^1, map[y][x1]^1);
        }

        // draw();
    };

    auto swap_col = [&](int x)
    {
        for (int y1 = rows/2; y1 < rows; ++y1)
        {
            const int y2 = rows-1-y1;
            tie(map[y1][x], map[y2][x]) = make_tuple(map[y2][x]^1, map[y1][x]^1);
        }

        // draw();
    };

    if (rows%2 == 1)
    {
        const int y = rows/2;

        for (int x = 0; x < cols; ++x)
            if (map[y][x] == 0)
            {
                answer << "C " << x+1 << '\n';
                swap_col(x);
            }
    }

    if (cols%2 == 1)
    {
        const int x = cols/2;

        for (int y = 0; y < rows; ++y)
            if (map[y][x] == 0)
            {
                answer << "R " << y+1 << '\n';
                swap_row(y);
            }
    }

    auto swap_all_if_needed= [&](int y1, int x1, int y2, int x2)
    {
        if (map[y1][x1] + map[y2][x2] != 0)
            return;

        answer << "R " << y1+1 << '\n'
               << "R " << y2+1 << '\n'
               << "C " << x1+1 << '\n'
               << "R " << y1+1 << '\n'
               << "R " << y2+1 << '\n'
               << "C " << x1+1 << '\n';

#ifdef ADDITIONAL_LOGS
        swap_row(y1);
        swap_row(y2);
        swap_col(x1);
        swap_row(y1);
        swap_row(y2);
        swap_col(x1);
#else
        map[y1][x1] ^= 1;
        map[y1][x2] ^= 1;
        map[y2][x1] ^= 1;
        map[y2][x2] ^= 1;
#endif
    };

    auto swap_if_needed = [&](int y1, int x1, int y2, int x2)
    {
        if (map[y1][x1] + map[y2][x2] != 0)
            return;

        if (y1 == y2)
            answer << "R " << y1+1 << '\n'
                   << "C " << x1+1 << '\n'
                   << "R " << y1+1 << '\n'
                   << "C " << x1+1 << '\n';
        else
            answer << "C " << x1+1 << '\n'
                   << "R " << y1+1 << '\n'
                   << "C " << x1+1 << '\n'
                   << "R " << y1+1 << '\n';

#ifdef ADDITIONAL_LOGS
        if (y1 == y2)
        {
            swap_row(y1);
            swap_col(x1);
            swap_row(y1);
            swap_col(x1);
        }
        else
        {
            swap_col(x1);
            swap_row(y1);
            swap_col(x1);
            swap_row(y1);
        }
#else
        map[y1][x1] ^= 1;
        map[y2][x2] ^= 1;
#endif
    };

    for (int y = 0; y < rows/2; ++y)
        for (int x = 0; x < cols/2; ++x)
        {
            const int y2 = rows-1-y;
            const int x2 = cols-1-x;

            swap_all_if_needed(y, x, y2, x2);
            swap_if_needed(y, x, y, x2);
            swap_if_needed(y, x, y2, x);
            swap_if_needed(y2, x2, y, x2);
            swap_if_needed(y2, x2, y2, x);

            if (map[y][x] + map[y][x2] + map[y2][x] + map[y2][x2] != 4)
            {
                cout << "NO\n";
                return;
            }
        }

#ifdef ADDITIONAL_LOGS
    draw();
#endif

    cout << answer.str();
}

/******************************************************************************/

int main()
{
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

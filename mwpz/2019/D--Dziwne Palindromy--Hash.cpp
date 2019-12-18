#include <bits/stdc++.h>
#define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define MEMORY_LIMIT 256
#define TIME_LIMIT 15.0
// #define ADDITIONAL_LOGS // enable blocks used only for cerr

using namespace std;


template<typename T, int HEIGHT, int WIDTH>
using my_array = array<array<T, WIDTH>, HEIGHT>;
using ull = unsigned long long;

const ull MUL = 29;

my_array<char, 256, 256> buffer;


my_array<ull, 256, 256> hash_top;
my_array<ull, 256, 256> hash_bottom;
my_array<ull, 256, 256> hash_left;
my_array<ull, 256, 256> hash_right;

array<ull, 256> pows = []()
{
    array<ull, 256> pows;
    pows[0] = 1;
    for (int i = 1; i < 256; ++i)
        pows[i] = MUL * pows[i-1];

    return pows;
}();

void test()
{        
    int height, width;

    cin >> height >> width;

    for (int y = 0; y < height; ++y)
        cin >> buffer[y].data();


#ifndef CONTEST_WORKSPACE
    const bool was_simple = [&]()
    {
        int mask = 0;

        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x)
                mask |= (1<<(buffer[y][x]-'a'));

        if (__builtin_popcount(mask) > 1)
            return false;

        int weird_palindromes = 0;

        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x)
            {
                const int len_y = min(y, height-1-y);
                const int len_x = min(x, width-1-x);

                const int main_y = y;
                const int main_x = x;

                weird_palindromes += 1 + main_x * (2*len_y+1) + main_y * (2*len_x+1);
            }

        cout << weird_palindromes << '\n';

        return true;
    }();

    if (was_simple)
        return;
#endif

    auto compute_hashes = [](auto rows, auto cols, auto data, auto hash)
    {
        for (int x = 0; x < cols; ++x)
        {
            hash(-1, x) = 0;
            for (int y = 0; y < rows; ++y)
            {
                const ull c = data(y, x) - 'a';
                hash(y, x) = MUL * hash(y-1, x) + c;
            }
        }
    };

    compute_hashes(height, width, 
        [=](int y, int x) { return buffer[y][x]; }, 
        [=](int y, int x) -> ull& { return hash_top[y+1][x]; });

    compute_hashes(height, width, 
        [=](int y, int x) { return buffer[height-1-y][x]; }, 
        [=](int y, int x) -> ull& { return hash_bottom[height-y][x]; });

    compute_hashes(width, height, 
        [=](int x, int y) { return buffer[y][x]; },
        [=](int x, int y) -> ull& { return hash_left[y][x+1]; });

    compute_hashes(width, height, 
        [=](int x, int y) { return buffer[y][x]; }, 
        [=](int x, int y) -> ull& { return hash_right[y][width-x]; });


    int weird_palindromes = 0;

    auto solve_orientation = [&weird_palindromes](auto hash1, auto hash2, auto at, auto height, auto width)
    {
        auto solve_weird = [&](int y, int x1, int x2)
        {
            int half_len = [&]()
            {
                int min_len = 0;
                int max_len = std::min(y, height-1-y);
                while (min_len+5 < max_len)
                {
                    const int test_len = (min_len+max_len+1)/2;

                    const bool ok = (hash1(y, x1, test_len) == hash2(y, x2, test_len));

                    if (ok)
                        min_len = test_len;
                    else
                    {
                        max_len = test_len-1;
                    }
                    
                }
                return min_len;
            }();

            weird_palindromes += half_len;

            while (true)
            {
                half_len += 1;

                const int y1 = y-half_len;
                const int y2 = y+half_len;
                if (y1 < 0 || y2 >= height || at(y1, x1) != at(y2, x2))
                    break;

                weird_palindromes += 1;
            }
        };

        auto solve = [&](int y, int x, int base_len)
        {
            int half_len = 0;

            while (true)
            {
                half_len += 1;

                const int x1 = x-half_len;
                const int x2 = x+base_len+half_len;
                if (x1 < 0 || x2 >= width || at(y, x1) != at(y, x2))
                    break;

                solve_weird(y, x1, x2);
                solve_weird(y, x2, x1);

                weird_palindromes += 1;
            }
        };

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                solve(y, x, -1);
                solve(y, x, 0);
            }
        }
    };


    auto hash1 = [](int y, int x, int len)
    {
        return hash_top[y+1][x] - hash_top[y+1-len][x] * pows[len];
    };

    auto hash2 = [](int y, int x, int len)
    {
        return hash_bottom[y+1][x] - hash_bottom[y+1+len][x] * pows[len];
    };

    auto hash3 = [](int x, int y, int len)
    {
        return hash_left[y][x+1] - hash_top[y][x+1-len] * pows[len];
    };

    auto hash4 = [](int x, int y, int len)
    {
        return hash_right[y][x+1] - hash_right[y][x+1+len] * pows[len];
    };

    auto at1 = [](int y, int x) { return buffer[y][x]; };
    auto at2 = [](int x, int y) { return buffer[y][x]; };

    solve_orientation(hash1, hash2, at1, height, width);
    solve_orientation(hash3, hash4, at2, width, height);
    cout << weird_palindromes + height*width << '\n';
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

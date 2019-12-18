#include <bits/stdc++.h>
#define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define MEMORY_LIMIT 256
#define TIME_LIMIT 15.0
// #define ADDITIONAL_LOGS // enable blocks used only for cerr

using namespace std;


template<typename T, int HEIGHT, int WIDTH>
using my_array = array<array<T, WIDTH>, HEIGHT>;

my_array<char, 256, 256> buffer;
array<int, 512> pairwise_lcp;
my_array<int, 256, 256> lcp;

void test()
{        
    int height, width;

    cin >> height >> width;

    for (int y = 0; y < height; ++y)
        cin >> buffer[y].data();

#if 0
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

    int weird_palindromes = 0;

    auto solve_orientation = [&weird_palindromes](auto at, auto height, auto width)
    {
        auto precompute = [&](int y, int len)
        {
            // cout << "precompute(y=" << y << ", len=" << len << ")" << endl;

            struct Word {
                int x;
                char y_step;
            };
            
            auto word_at = [at, y](const Word &word, int i) 
            {
                return at(y+word.y_step*(i+1), word.x);
            };

            std::vector<Word> words;
            words.reserve(2*width);

            for (int x = 0; x < width; ++x)
            {
                words.push_back({x, -1});
                words.push_back({x, 1});
            }

            for (int i = len-1; i >= 0; --i)
            {
                array<int, 27> counts;
                fill(counts.begin(), counts.end(), 0);

                for (auto &word : words)
                    counts[word_at(word, i) - 'a' + 1] += 1;

                partial_sum(counts.begin(), counts.end(), counts.begin());

                std::vector<Word> sorted(words.size());

                for (auto &word : words)
                {
                    auto &pos = counts[word_at(word, i) - 'a'];
                    sorted[pos] = word;
                    pos += 1;
                }

                sorted.swap(words);
            }

            const int n = 2*width;
            //vector<int> pairwise_lcp(n, -1);

            for (int w = 0; w < n-1; ++w)
            {
                int common_prefix = 0;

                while (common_prefix < len && 
                       word_at(words[w], common_prefix) == word_at(words[w+1], common_prefix))
                {
                    common_prefix += 1;
                }

                pairwise_lcp[w] = common_prefix;
            }

            // for (int i = 0; i < n; ++i)
            // {
            //     for (int j = 0; j < len; ++j)
            //         cout << word_at(words[i], j);
            //     cout << "   " << pairwise_lcp[i] << endl;
            // }

            // vector<vector<int>> lcp(width, vector<int>(width));

            for (int lhs = 0; lhs < n; ++lhs)
            {
                const int x1 = words[lhs].x;
                const int step1 = words[lhs].y_step;


                int common_prefix = len;
                
                for (int rhs = lhs+1; rhs < n; ++rhs)
                {
                    common_prefix = min(common_prefix, pairwise_lcp[rhs-1]);

                    const int step2 = words[rhs].y_step;
                    if (step1 != step2)
                    {
                        const int x2 = words[rhs].x;

                        if (step1 == 1)
                            lcp[x1][x2] = common_prefix;
                        else
                            lcp[x2][x1] = common_prefix;
                    }
                }
            }

            auto solve = [&](int y, int x, int base_len, auto &lcp)
            {
                int half_len = 0;

                while (true)
                {
                    half_len += 1;

                    const int x1 = x-half_len;
                    const int x2 = x+base_len+half_len;
                    if (x1 < 0 || x2 >= width || at(y, x1) != at(y, x2))
                        break;

                    // cout << "palindrome y=" << y << " x=" << x1 << ".." << x2 << "   ^v = " << lcp[x2][x1] << "  v^ = " << lcp[x1][x2] << endl;

                    weird_palindromes += 1 + lcp[x1][x2] + lcp[x2][x1];
                }
            };

            for (int x = 0; x < width; ++x)
            {
                solve(y, x, -1, lcp);
                solve(y, x, 0, lcp);
            }
        };

        for (int y = 0; y < height; ++y)
        {
            const int len = std::min(y, height-1-y);
            precompute(y, len);
        }
    };

    auto at1 = [](int y, int x) { return buffer[y][x]; };
    auto at2 = [](int x, int y) { return buffer[y][x]; };

    solve_orientation(at1, height, width);
    solve_orientation(at2, width, height);
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

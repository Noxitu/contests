#include <bits/stdc++.h>
// #define MULTIPLE_TESTS
// #define ENDLESS_TESTS
// #define MEMORY_LIMIT 128

using namespace std;

template<int POW, int MOD>
class SingleHash
{
private:
    static vector<int> hash_powers;
    int hash_value = 0;

public:
    int value() const noexcept { return hash_value; }

    void update(int power, bool sign) noexcept
    {
        if (sign)
        {
            hash_value = (hash_value - 1LL * hash_powers[power] % MOD + MOD) % MOD;
        }
        else
            hash_value = (hash_value + 1LL * hash_powers[power]) % MOD;
    }

    static void precompute_hash_powers(int n)
    {
        hash_powers.resize(n);
        hash_powers[0] = 1;

        for (int i = 1; i < n; ++i)
            hash_powers[i] = 1LL * hash_powers[i-1] * POW % MOD;
    }

    friend bool operator==(const SingleHash<POW, MOD> &lhs, const SingleHash<POW, MOD> &rhs)
    {
        return lhs.value() == rhs.value();
    }

    std::size_t hash() const noexcept
    {
        return std::hash<int>{}(hash_value);
    }
};

template<typename ...Hashes>
class MultiHash;

template<typename Hash, typename ...Hashes>
class MultiHash<Hash, Hashes...>
{
private:
    Hash first;
    MultiHash<Hashes...> other;

public:
    void update(int power, bool sign) noexcept
    {
        first.update(power, sign);
        other.update(power, sign);
    }

    static void precompute_hash_powers(int n)
    {
        Hash::precompute_hash_powers(n);
        MultiHash<Hashes...>::precompute_hash_powers(n);
    }

    std::size_t hash() const noexcept
    {
        return first.hash() ^ other.hash();
    }

    friend bool operator==(const MultiHash &lhs, const MultiHash &rhs)
    {
        return lhs.first == rhs.first && lhs.other == rhs.other;
    }
};

template<>
class MultiHash<> 
{
public:
    void update(int, bool) noexcept {}

    static void precompute_hash_powers(int) {}

    std::size_t hash() const noexcept { return {}; }

    friend bool operator==(const MultiHash &, const MultiHash &)
    {
        return true;
    }
};

template<int POW, int MOD>
vector<int> SingleHash<POW, MOD>::hash_powers;


namespace std
{
    template<int POW, int MOD> struct hash<SingleHash<POW, MOD>>
    {
        std::size_t operator()(const SingleHash<POW, MOD> &s) const noexcept
        {
            return s.hash();
        }
    };

    template<typename ...Hashes> struct hash<MultiHash<Hashes...>>
    {
        std::size_t operator()(const MultiHash<Hashes...> &s) const noexcept
        {
            return s.hash();
        }
    };
}

constexpr int MASK = 0x10000000;

using Hash = MultiHash<
     SingleHash<3, 879872831>
    ,SingleHash<5, 602422663>
    ,SingleHash<7, 1603996861>
    ,SingleHash<11, 1891071893>
    ,SingleHash<13, 1537742641>
    ,SingleHash<17, 1859222929>
    ,SingleHash<19, 1583468923>
    ,SingleHash<23, 1861187689>
>;

void test()
{
    int n, width, height;
    cin >> n >> width >> height;

    Hash::precompute_hash_powers(n);

    vector<pair<int, int>> xs(2*n+1);
    vector<pair<int, int>> ys(2*n+1);
    
    for (int i = 0; i < n; ++i)
    {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        xs[i] = {x1, i};
        xs[i+n] = {x2, i|MASK};
        ys[i] = {y1, i};
        ys[i+n] = {y2, i|MASK};
    }

    xs[2*n] = {width, -1};
    ys[2*n] = {height, -1};
    
    auto solve = [&](auto &vec)
    {
        int pos = 0;
        Hash hash;
        sort(vec.begin(), vec.end());

        unordered_map<Hash, int> solutions;
        solutions.reserve(2*n);

        for (const auto &change : vec)
        {
            const int new_pos = change.first;

            if (new_pos > pos)
            {
                solutions[hash] += (new_pos - pos);
                pos = new_pos;
            }

            if (change.second == -1)
                break;

            const bool remove = (change.second&MASK) == MASK;
            const int i = change.second&~MASK;

            hash.update(i, remove);
        }

        int best = 0;
        for (auto &solution : solutions)
        {
            best = max(best, solution.second);
        }

        return best;
    };

    cout << 1LL * solve(xs) * solve(ys) << '\n';
}

/******************************************************************************/

int main()
{
#if defined(CONTEST_WORKSPACE) && defined(MEMORY_LIMIT)
    void limit_ram(float);
    limit_ram(MEMORY_LIMIT);
#endif

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

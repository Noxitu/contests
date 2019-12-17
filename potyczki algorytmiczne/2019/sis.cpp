#include <bits/stdc++.h>
// #define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define MEMORY_LIMIT 128
#define TIME_LIMIT 10.0
// #define ADDITIONAL_LOGS // enable blocks used only for cerr

using namespace std;

constexpr long long END = 2'000'000'000'000'000'000 + 2;

struct Query
{
    int type;
    long long size, target;
};

class GloballyClearableFlag
{
private:
    static int current_index;
    int index = -1;
public:
    void operator=(bool b)
    {
        index = b ? current_index : -1;
    }

    operator bool() const
    {
        return index == current_index;
    }

    static void reset_all()
    {
        current_index += 1;
    }
};
int GloballyClearableFlag::current_index = 0;

struct Tree
{
    long long value;
    long long sum;
    int count;
    GloballyClearableFlag used = {};
};

ostream &operator<<(ostream &out, const Tree &val)
{
    return out << '(' << val.sum << ", " << val.count << ") ";
}

void test()
{
    multiset<long long> current;
    map<long long, int> possible;

    current.insert(END);
    possible[END] = 1;

    int n;
    cin >> n;

    for (int i = 0; i < n; ++i)
    {
        long long size;
        cin >> size;
        current.insert(size);
        possible[size] += 1;
    }

    int possible_space = n+1;

    int q;
    cin >> q;
    vector<Query> queries(q);

    for(auto &query : queries)
    {
        cin >> query.type >> query.size;
        if (query.type == 1)
            cin >> query.target;
        if (query.type == 2)
        {
            possible[query.size] += 1;
            possible_space += 1;
        }
    }

    n = 1;
    while (n < possible_space)
        n *= 2;

    // cerr << "n = " << n << endl;

    vector<Tree> tree;
    tree.reserve(2*n-1);
    tree.resize(n-1);

    auto parent = [](int index) { return (index-1)/2; };
    auto left = [](int index) { return 2*index + 1; };
    auto right = [](int index) { return 2*index + 2; };
    auto is_leftmost = [](int index) { return (index & (index+1)) == 0; };
    auto is_leaf = [=](int index) { return index >= n-1; };
    auto is_right_child = [=](int index) { return index > 0 && index%2 == 0; };

    for (const auto &p : possible)
    {
        const long long size = p.first;
        const int possible_count = p.second;
        const int current_count = current.count(size);
        const int empty_count = possible_count - current_count;

        for (int i = 0; i < empty_count; ++i)
            tree.push_back({size, 0, 0});

        for (int i = empty_count; i < possible_count; ++i)
            tree.push_back({size, size, 1});
    }

    for (int i = n-2; i >= 0; --i)
    {
        tree[i].sum = tree[left(i)].sum + tree[right(i)].sum;
        tree[i].count = tree[left(i)].count + tree[right(i)].count;
    }

    auto leaf = tree.begin() + (n-1);
    possible.clear();

    stack<int> stack;

    auto mark = [&](const int index)
    {
        for (size_t i = index; !tree[i].used; i = parent(i))
        {
            tree[i].used = true;

            if (i == 0)
                break;
        }

        for (size_t i = index; i < tree.size(); i = right(i))
        {
            tree[i].used = true;
        }
    };

    auto eat = [&](long long &size, long long target, int &count)
    {
        while (size < target)
        {
            // cerr << "size=" << size << "  target=" << target << endl;
            if (stack.empty())
                break;

            int index = stack.top();
            stack.pop();

            if (is_leftmost(index))
                continue;

            index -= 1;

            if (tree[index].used)
                continue;

            while (is_right_child(index) &&
                   tree[parent(index)].used == false &&
                   size + tree[parent(index)].sum <= target)
            {
                assert(tree[parent(index)].sum == tree[index].sum + tree[index-1].sum);
                assert(tree[parent(index)].count == tree[index].count + tree[index-1].count);
                // cerr << "parent is better than eating size=" << size << "  count=" << count << endl;
                index = parent(index);
            }

            // cerr << "eating index=" << index << "  sum=" << tree[index].sum << "  count=" << tree[index].count << endl;
            size += tree[index].sum;
            count += tree[index].count;
            mark(index);

            while (!is_leaf(index))
                index = left(index);

            stack.push(index);
        }
    };
    
    auto solve = [&](long long current_size, long long final_goal)
    {
        GloballyClearableFlag::reset_all();
        // cerr << "solve " << current_size << ' ' << final_goal << endl;
        while(!stack.empty()) stack.pop();

        int count = 0;

        while (current_size < final_goal)
        {
            const long long next_size = *current.lower_bound(current_size);
            const long long next_target = min(next_size+1, final_goal);
            auto it = std::lower_bound(leaf, tree.end(), current_size, [](Tree &t, long long value)
            {
                return t.value < value;
            });

            if (it == leaf)
                break;

            assert((it-1)->value < current_size);

            stack.push(it-tree.begin());

            eat(current_size, next_target, count);

            if (current_size < next_target)
                break;
        }
        if (current_size < final_goal)
            cout << -1 << '\n';
        else
            cout << count << '\n';
    };

    auto updated = [&](int i)
    {
        while (i > 0)
        {
            i = parent(i);
            tree[i].sum = tree[left(i)].sum + tree[right(i)].sum;
            tree[i].count = tree[left(i)].count + tree[right(i)].count;
        }
    };

    auto add = [&](long long size)
    {
        // cerr << "add " << size << endl;
        current.insert(size);

        auto it = std::lower_bound(leaf, tree.end(), size, [](Tree &t, long long value)
        {
            if (t.value == value)
                return t.count < 1;
            return t.value < value;
        });

        --it;

        assert(it->value == size);
        assert(it->sum == 0);
        assert(it->count == 0);

        it->sum = size;
        it->count = 1;

        const int index = (it-leaf) + (n-1);

        updated(index);
    };

    auto remove = [&](long long size)
    {
        // cerr << "remove " << size << endl;
        current.erase(current.find(size));

        auto it = std::lower_bound(leaf, tree.end(), size, [](Tree &t, long long value)
        {
            if (t.value == value)
                return t.count < 1;
            return t.value < value;
        });

        assert(it->value == size);
        assert(it->sum == size);
        assert(it->count == 1);

        it->sum = 0;
        it->count = 0;

        const int index = (it-leaf) + (n-1);

        updated(index);
    };

    for (const auto &query : queries)
    {
        if (query.type == 1) solve(query.size, query.target);        
        if (query.type == 2) add(query.size);
        if (query.type == 3) remove(query.size);

#ifdef ADDITIONAL_LOGS
        int enter_at = 1;

        for (int i = 0; i < 2*n-1; ++i)
        {
            // cerr << tree[i];

            if (i+1 == enter_at)
            {
                // cerr << endl;
                enter_at = 2*enter_at+1;
            }
        }

        for (auto v : current)
        {
            // cerr << v << ' ';
        }
        // cerr << endl;
#endif
    }
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

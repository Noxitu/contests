#include <bits/stdc++.h>
// #define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define MEMORY_LIMIT 128
#define TIME_LIMIT 3.0

using namespace std;

constexpr long long END = 2'000'000'000'000'000'000 + 2;

int current_used_value = 0;

struct Tree {
    Tree *up = nullptr;
    Tree *left = nullptr;
    Tree *right = nullptr;
    Tree *first = nullptr;
    Tree *last = nullptr;

    long long sum = 0;
    long long max = 0;
    int count = 0;
    struct {
        int used_value = -1;
        operator bool() const {
            return current_used_value == used_value;
        }
        void operator= (bool b)
        {
            used_value = b ? current_used_value : -1;
        }
    } used;
};

void test()
{
    // cerr << "hi" << endl;
    deque<Tree> tree;

    {
        auto push_tree = [&](long long val)
        {
            Tree *left = tree.empty() ? nullptr : &tree.back();

            tree.push_back({});
            Tree *me = &tree.back();

            if (left)
                left->right = me;
            me->left = left;

            me->sum = val;
            me->max = val;
            me->count = 1;
        };

        int n;
        cin >> n;

        vector<long long> values(n);
        for (auto &val : values)
            cin >> val;

        sort(values.begin(), values.end());

        for (auto val : values)
        {
            push_tree(val);
        }
        push_tree(END);
    }

    auto root = &tree.back();

    while (true)
    {
        auto it = root;
        while (it->left)
            it = it->left;

        if (it == root)
            break;

        Tree *prev = nullptr;
        while (it)
        {
            tree.push_back({});
            Tree *me = &tree.back();

            me->left = prev;
            if (prev)
                prev->right = me;
            prev = me;
            me->first = it;

            it->up = me;
            me->last = it;
            me->sum += it->sum;
            me->max = std::max(me->max, it->max);
            me->count += it->count;
            it = it->right;

            if (!it) break;

            it->up = me;
            me->last = it;
            me->sum += it->sum;
            me->max = std::max(me->max, it->max);
            me->count += it->count;
            it = it->right;
        }

        root = &tree.back();
    }

    auto find_greater_or_equal = [&](long long value)
    {
        auto e = root;
        while (e->last)
        {
            while(e->left && e->left->max >= value)
                e = e->left;
            e = e->last;
        }

        while(e->left && e->left->max >= value)
                e = e->left;
        
        return e;
    };

    stack<Tree*> stack;

    auto eat = [&](long long &size, long long target_size, int &count)
    {
        while (size < target_size)
        {
            if (stack.empty()) return false;

            auto e = stack.top();
            stack.pop();

            auto use = [&](auto e)
            {
                size += e->sum;
                count += e->count;
                // cerr << "Eating #" << e->count << " size=" << e->sum << "  szczupak=" << size << endl;
                while (e && !e->used)
                {
                    e->used = true;
                    e = e->up;
                }
            };

            const bool add = [&]()
            {
                while (size < target_size)
                {
                    e = e->left;

                    if (!e) return false;

                    // cerr << ":: starting with sum = " << e->sum << endl;

                    while(e->up && 
                          size + e->sum <= target_size &&
                          e->up->last == e &&
                          !e->up->used)
                        e = e->up;

                    // cerr << ":: now with sum = " << e->sum << endl;

                    // cerr << (bool) e->up;
                    // cerr << (size + e->sum <= target_size);
                    // cerr << (e->up && e->up->last == e);
                    // cerr << (e->up && !e->up->used) << endl;

                    while (e->last &&
                           (e->used || 
                            size + e->sum > target_size))
                        e = e->last;

                    // cerr << (bool) e;
                    // cerr << (e &&e->used);
                    // cerr << (e && (size + e->sum > target_size)) << endl;

                    if (e->used) return false;

                    // cerr << ":: ended with sum = " << e->sum << endl;

                    use(e);
                }

                return true;
            }();

            if (add)
            {
                stack.push(e);
            }
        }

        return true;
    };
    
    auto query = [&]()
    {
        long long current_size, final_goal;
        cin >> current_size >> final_goal;
        // cerr << "query " << current_size << ' ' << final_goal << endl;

        // for (auto &t : tree) t.used = false;
        while(!stack.empty()) stack.pop();

#if 0
        // cerr << "Start size = " << current_size << endl;
        // cerr << "final goal = " << final_goal << endl;
        {
            auto e2 = root;
            while(e2)
            {
                auto e = e2;    
                while(e)
                {
                    // cerr << e->sum << ' ';
                    e = e->right;
                }
                // cerr << endl;

                e2 = e2->first;
            }
        }
#endif
        int count = 0;

        while (current_size < final_goal)
        {
            auto e = find_greater_or_equal(current_size);
            const long long x = min(final_goal, e->max+1);
            // cerr << "Trying to eat until " << x << endl;
            // cerr << "size = " << current_size << endl;
            stack.push(e);

            const bool ok = eat(current_size, x, count);
            if (!ok) break;
        }

        // cerr << "End size = " << current_size << endl;
        // cerr << "count = " << count << endl;

        if (current_size < final_goal)
            cout << -1 << '\n';
        else
            cout << count << '\n';

        // cerr << endl;
    };

    auto recompute = [&](Tree *me)
    {
        while(me)
        {
            me->sum = 0;
            me->max = 0;
            me->count = 0;

            auto e = me->first;
            while (true)
            {
                me->sum += e->sum;
                me->max = max(me->max, e->max);
                me->count += e->count;

                if (e == me->last)
                    break;

                e = e->right;
            }

            me = me->up;
        }
    };

    auto add = [&]()
    {
        long long w;
        cin >> w;
        // cerr << "add " << w << endl;
        auto e = find_greater_or_equal(w);
        
        tree.push_back({});
        auto me = &tree.back();
        
        me->left = e->left;
        me->right = e;
        if (me->left)
            me->left->right = me;
        e->left = me;

        me->sum = w;
        me->max = w;
        me->count = 1;

        me->up = me->right->up;
        if (me->up && me->up->first == me->right)
            me->up->first = me;

        recompute(me->up);
    };

    auto remove_node = [&](Tree *me)
    {
        while (true)
        {
            auto right = me->right;
            auto left = me->left;
            
            if (left)
                left->right = right;
            right->left = left;

            assert(me->up);

            if (me->up->first == me->up->last)
            {
                me = me->up;
                continue;
            }

            if (me == me->up->first)
                me->up->first = me->right;

            if (me == me->up->last)
                me->up->last = me->left;

            recompute(me->up);
            break;
        }
    };

    auto remove = [&]()
    {
        long long w;
        cin >> w;
        // cerr << "remove " << w << endl;
        auto me = find_greater_or_equal(w);
        remove_node(me);
    };

    int q;
    cin >> q;
    while (q --> 0)
    {
        int type;
        cin >> type;
        switch (type)
        {
            case 1:
                query();
                break;
            case 2:
                add();
                break;
            case 3:
                remove();
                break;
        }
        current_used_value += 1;
    }
}

/******************************************************************************/

int main()
{
#ifdef CONTEST_WORKSPACE
    #ifdef MEMORY_LIMIT
        void limit_ram(float);
        limit_ram(MEMORY_LIMIT);
    #endif
    #ifdef TIME_LIMIT
        void limit_cpu(float);
        limit_cpu(TIME_LIMIT);
    #endif
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

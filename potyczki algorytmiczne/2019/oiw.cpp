#include <bits/stdc++.h>
// #define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define MEMORY_LIMIT 400
#define TIME_LIMIT 5.0
// #define ADDITIONAL_LOGS // enable blocks used only for cerr

using namespace std;

struct Point {
    int x, y;
};

struct CompareByY
{ 
    bool operator() (const Point &lhs, const Point &rhs) const
    {
        return lhs.y < rhs.y;
    }
};

struct CompareByYX
{ 
    bool operator() (const Point &lhs, const Point &rhs) const
    {
        if (lhs.y == rhs.y)
            return lhs.x < rhs.x;
        return lhs.y < rhs.y;
    }
};

struct CompareByXY
{ 
    bool operator() (const Point &lhs, const Point &rhs) const
    {
        if (lhs.x == rhs.x)
            return lhs.y < rhs.y;
        return lhs.x < rhs.x;
    }
};

class Structure
{
private:
    set<Point, CompareByY> m_stack;
    set<Point, CompareByYX> m_rows;
    set<Point, CompareByXY> m_cols;

public:
    Structure(int width, int height)
    {
        m_stack.insert({-1, -1});
        m_stack.insert({width, 0});
        m_stack.insert({width+1, height});
        m_rows.insert({width, height});
        m_cols.insert({width, height});
    }

    bool is(int x, int y) const
    {
        auto next = m_stack.lower_bound({0, y});
        auto prev = ::prev(next);

        if (x == prev->x-1)
            return true;

        return y == next->y && x >= prev->x && x < next->x;
    }

    void expand(int x, int y)
    {
        assert(is(x, y));
        const auto end = m_stack.upper_bound({0, y});
        auto begin = end;

        int x2 = prev(end)->x;

        while (begin->x >= x)
            --begin;
        ++begin;

        int y1 = begin->y;

        m_stack.erase(begin, end);

        m_stack.insert({x, y1});
        if (end->y != y+1)
            m_stack.insert({x2, y+1});
    }

    void add(int x, int y)
    {
        m_rows.insert({x, y});
        m_cols.insert({x, y});
    }

    void search(int x, int y, queue<Point> &points)
    {
        const auto next = m_stack.upper_bound({0, y});
        const auto prev = ::prev(next);

        assert(next->y == y+1);


        {
            const int the_x = x-1;
            const int first_y = prev->y+1;
            const int last_y = y+1;

            auto it = m_cols.lower_bound({the_x, first_y});

            while(it->x == the_x && it->y <= last_y)
            {
                assert(is(it->x, it->y));
                points.push(*it);

                m_rows.erase(*it);
                it = m_cols.erase(it);
            }
        }

        {
            const int first_x = x-1;
            const int last_x = next->x-1;
            const int the_y = y+1;

            auto it = m_rows.lower_bound({first_x, the_y});

            while(it->x <= last_x && it->y == the_y)
            {
                assert(is(it->x, it->y));
                points.push(*it);
                
                m_cols.erase(*it);
                it = m_rows.erase(it);
            }
        }
    }
};

void test()
{        
    int height, width, k;
    cin >> height >> width >> k;

    Structure top(width, height);
    Structure left(height, width);

    int x = 0;

    while (k-->0)
    {
        int ri, ci, zi;
        cin >> ri >> ci >> zi;

        const int r = (ri^x)%height;
        const int c = (ci^x)%width;

        const bool is1 = top.is(c, r);
        const bool is2 = left.is(r, c);

        assert(r != 0 || c != 0);
        assert(r != height-1 || c != width-1);

        if (is1 && is2)
        {
            cout << "TAK\n";
            x ^= zi;
            continue;
        }

        cout << "NIE\n";

        auto handle = [](Structure &s, bool is, int x, int y)
        {
            if (!is)
            {
                s.add(x, y);
                return;
            }
            
            queue<Point> expand;
            expand.push({x, y});
            while (!expand.empty())
            {
                const auto p = expand.front();
                expand.pop();

                // this can happen during chain reaction
                if (!s.is(p.x, p.y))
                    continue; 
                
                s.expand(p.x, p.y);
                s.search(p.x, p.y, expand);
            }
        };

        handle(top, is1, c, r);
        handle(left, is2, r, c);
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

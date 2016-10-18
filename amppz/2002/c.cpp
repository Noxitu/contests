#include "../biblioteczka.hpp"

int main() { return main_many(); }

struct Position
{
    int x, y;
    int score = 0;
    friend istream& operator>> (istream &in, Position &pos) { return in >> pos.x >> pos.y; }
};

class Sweep
{
    int Position::* coord;
    int current, total = 0, delta = 0;
public:
    Sweep(int Position::* coord) : coord(coord) {}
    void operator()(Position &pos)
    {
        total += abs(current-pos.*coord)*delta;
        current = pos.*coord;
        pos.score += total;
        ++delta;
    }
};

void test()
{
    int n;
    cin >> n;
    vector<Position> points(n);
    cin >> points;
    
    sort( begin(points), end(points), compare_field<Position, int, &Position::x>() );
    for_each( points.begin(), points.end(), Sweep(&Position::x));
    for_each( points.rbegin(), points.rend(), Sweep(&Position::x));
    
    sort( begin(points), end(points), compare_field<Position, int, &Position::y>() );
    for_each( points.begin(), points.end(), Sweep(&Position::y));
    for_each( points.rbegin(), points.rend(), Sweep(&Position::y));
    
    auto best = min_element( begin(points), end(points), compare_field<Position, int, &Position::score>() );
    cout << best->score << '\n';
}

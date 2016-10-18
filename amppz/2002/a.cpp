#include "../biblioteczka.hpp"

int main() { return main_many(); }

struct Road
{
    int v, u;
    int a, b;
    
    friend istream& operator>>(istream &in, Road &r) { return in >> r.v >> r.u >> r.a >> r.b; }
};

struct Result
{
    double cost;
    int change;
};

class UnionFind
{
    std::vector<int> parents;
    std::vector<int> ranks;
public:
    UnionFind(size_t size) : parents(size, -1), ranks(size, 0) {}
    size_t find(size_t id)
    {
        if(parents.at(id) == -1)
            return id;
            
        return parents[id] = find(parents[id]); 
    }
    void Union(size_t x, size_t y)
    {
        x = find(x);
        y = find(y);
        if(ranks[x] > ranks[y]) parents[y] = x;
        else if(ranks[x] < ranks[y]) parents[x] = y;
        else if(x != y)
        {
            parents[y] = x;
            ++ranks[x];
        }
    }
};

Result MinimumSpanningTree(double when, int n, std::vector<Road> &roads)
{
    sort(begin(roads), end(roads), [when](Road const &x, Road const &y) { return x.a*when+x.b < y.a*when+y.b; });
    
    UnionFind trees(n);
    
    Result result{0,0};
    for(auto &road : roads)
    {
        if(trees.find(road.u) != trees.find(road.v))
        {
            result.change += road.a;
            result.cost += road.b;
            trees.Union(road.v, road.u);
        }
    }
    
    result.cost += result.change*when;
    
    return result;
}

void test()
{
    int n, m;
    cin >> n >> m;
    
    std::pair<double, double> timespan;
    cin >> timespan;
    
    std::vector<Road> roads(m);
    cin >> roads;
    
    Result left_result = MinimumSpanningTree(timespan.first, n, roads);
    Result right_result = MinimumSpanningTree(timespan.second, n, roads);
    
    auto has_enough_precision = [&](double diff) { return diff < 1e-4 and fabs(diff*left_result.change) < 1e-4; };
    
    while( not has_enough_precision(timespan.second-timespan.first) )
    {
        double when = (timespan.first+timespan.second)/2;
        Result result = MinimumSpanningTree(when, n, roads);
        
        if(result.change <= 0)
        {
            timespan.second = when;
            right_result = result;
        }
        else
        {
            timespan.first = when;
            left_result = result;
        }
    }
    
    cout << fixed << setprecision(3) << timespan.first << ' ' << left_result.cost << '\n';
}
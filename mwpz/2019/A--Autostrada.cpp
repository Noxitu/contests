#include <bits/stdc++.h>
// #define MULTIPLE_TESTS
// #define ENDLESS_TESTS
#define MEMORY_LIMIT 256
#define TIME_LIMIT 15.0
// #define ADDITIONAL_LOGS // enable blocks used only for cerr

using namespace std;

struct LimitChange {
    int at, speed_limit;
};

struct Limit {
    int start, stop, speed_limit;
};

int acceleration;

struct MovementDescription {
    double acceleration_start;

    double instersect_with(double velocity) const
    {
        return velocity*velocity / (2.0*acceleration) + acceleration_start;
    }

    double at(double s) const
    {
        return sqrt(2.0*acceleration*abs(s-acceleration_start));
    }
};

enum MovementType {
    ACCELERATE, FLAT, SLOW
};

struct MovementRange {
    double start, stop, param;
    MovementType type;

    double at(double s) const
    {
        if (type == FLAT)
            return param;
        else
            return MovementDescription{param}.at(s);
    }

    double integral(double s) const
    {
        if (type == FLAT)
            return (1 / param) * (s-start);
        else
            return sqrt(2.0 * abs(s-param) / acceleration);
    }
};

void test()
{
    int n_limits, distance, max_velocity;
    cin >> n_limits >> distance >> acceleration >> max_velocity;

    auto limits = [&]()
    {
        vector<LimitChange> changes;
        changes.reserve(n_limits*2);


        while (n_limits --> 0)
        {
            int start, stop, speed_limit;
            cin >> start >> stop >> speed_limit;

            changes.push_back({start, speed_limit});
            if (stop < distance)
                changes.push_back({stop, -speed_limit});
        }

        sort(changes.begin(), changes.end(), [](auto &lhs, auto &rhs)
        {
            return lhs.at < rhs.at;
        });

        multiset<int> current_limits = {max_velocity};

        vector<Limit> ret;
        ret.push_back({0, distance, max_velocity});

        for (const auto &change : changes)
        {
            if (change.speed_limit > 0)
                current_limits.insert(change.speed_limit);
            else
                current_limits.erase(current_limits.find(-change.speed_limit));

            const int at = change.at;
            const int new_limit = *current_limits.begin();

            auto &current_limit = ret.back();

            if (current_limit.speed_limit == new_limit)
                continue;

            if (current_limit.start == at)
            {
                current_limit.speed_limit = new_limit;
                continue;
            }

            current_limit.stop = at;
            ret.push_back({at, distance, new_limit});
        }

        return ret;
    }();

    auto compute_movements = [&](const auto &limits, const auto CHANGE)
    {
        vector<MovementRange> movements;

        double current_start = 0;
        MovementDescription movement = {0};

        for (auto &speed_limit : limits)
        {
            const auto reached_at = movement.instersect_with(speed_limit.speed_limit);

            // cout << "Limit to " << speed_limit.speed_limit << " on " << speed_limit.start << ".." << speed_limit.stop << endl;
            // cout << "  Reached at " << reached_at << endl;

            if (reached_at < speed_limit.stop)
            {
                const double flat_since = max<double>(reached_at, speed_limit.start);
                if (current_start < flat_since)
                {
                    movements.push_back({current_start, flat_since, movement.acceleration_start, CHANGE});
                }

                movements.push_back({flat_since, static_cast<double>(speed_limit.stop), static_cast<double>(speed_limit.speed_limit), FLAT});
                current_start = speed_limit.stop;
                
                movement.acceleration_start += (speed_limit.stop-reached_at);
            }
            // cout << endl;
        }

        if (current_start < distance)
            movements.push_back({current_start, static_cast<double>(distance), movement.acceleration_start, CHANGE});

        if (CHANGE == SLOW)
        {
            reverse(movements.begin(), movements.end());
            for (auto &mov : movements)
            {
                tie(mov.start, mov.stop) = make_pair(distance-mov.stop, distance-mov.start);
                if (mov.type == SLOW)
                    mov.param = distance - mov.param;
            }
        }

        return movements;
    };

    const auto movements1 = compute_movements(limits, ACCELERATE);

    reverse(limits.begin(), limits.end());
    for (auto &speed_limit : limits)
        tie(speed_limit.start, speed_limit.stop) = make_pair(distance-speed_limit.stop, distance-speed_limit.start);

    const auto movements2 = compute_movements(limits, SLOW);

    assert(!movements1.empty());
    assert(!movements2.empty());

    double time = 0.0;
    auto it1 = movements1.begin();
    auto it2 = movements2.begin();

#if 0
    const double step = distance / 1e7;
    
    for (double x = step/2; x < distance; x += step)
    {
        while (x > it1->stop) ++it1;
        while (x > it2->stop) ++it2;

        const double v1 = it1->at(x);
        const double v2 = it2->at(x);
        const double v = min(v1, v2);

        time += (1.0 / v) * step;
    }
#else
    for (double x = 0; x < distance;)
    {
        while (x >= it1->stop) ++it1;
        while (x >= it2->stop) ++it2;

        const double stop = min(it1->stop, it2->stop);
        const double mid = (it1->param + it2->param) / 2;  // has sense only when neither is FLAT

        time += [&]() -> double
        {
            if (it2->type == FLAT)
                return it1->integral(stop) - it1->integral(x);

            if (it1->type == FLAT)
                return it2->integral(x) - it2->integral(stop);

            if (mid <= x)
                return it2->integral(x) - it2->integral(stop);
            
            if (mid >= stop)
                return it1->integral(stop) - it1->integral(x);

            return it1->integral(mid) - it1->integral(x) +
                   it2->integral(mid) - it2->integral(stop);
        }();

        x = stop;
    }
#endif

    cout << fixed << setprecision(9) << 60*time << '\n';
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

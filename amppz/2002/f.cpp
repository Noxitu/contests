#include "../biblioteczka.hpp"

int main() { return main_many(); }

struct Office;

struct Patient
{
    int time;
    vector<int> offices;
    
    friend istream& operator>> (istream &in, Patient &p)
    {
        size_t count;
        in >> p.time >> count;
        p.offices.resize(count);
        return in >> p.offices;
    }
};

struct Office
{
    int empty_at = 0;
};

struct Event;
typedef function<void(Event const&)> EventHandler;

struct Event
{   
    int time;
    int patient;
    EventHandler handle;
    Event(int time, int patient, EventHandler handler) : time(time), patient(patient), handle(handler) {}
    
    friend bool operator<(Event const &lhs, Event const &rhs)
    {
        if(lhs.time != rhs.time) return lhs.time > rhs.time;
        return lhs.patient > rhs.patient;
    }
};

void test()
{
    int n, m;
    cin >> n >> m;
    
    vector_from_one<Patient> patients(n);
    vector_from_one<Office> offices(m);
    cin >> patients;
    
    vector_from_one<vector<int>::const_iterator> currently_at(n);
    transform( begin(patients), end(patients), begin(currently_at), [](Patient const &p) { return begin(p.offices); } );
    
    priority_queue<Event> events;
    
    
    EventHandler arrive, leave;

    arrive = [&](Event const &event)
    {
        auto &next_office = currently_at[event.patient];
        auto &office = offices[*next_office];
        int enter = max(event.time, office.empty_at);
        
        office.empty_at = enter+1;
        events.emplace(enter+1, event.patient, leave);
    };
    
    leave = [&](Event const &event)
    {
        auto &patient = patients[event.patient];
        auto &next_office = currently_at[event.patient];
        
        if( ++next_office != end(patient.offices) )
            events.emplace(event.time, event.patient, arrive);
    };
    
    for(int p = 1; p <= n; ++p)
        events.emplace(patients[p].time, p, arrive);
        
    int time = 0;
    while(not events.empty())
    {
        const Event event = events.top();
        events.pop();
        
        event.handle(event);
        time = event.time;
    }
        
    cout << time << '\n';
}

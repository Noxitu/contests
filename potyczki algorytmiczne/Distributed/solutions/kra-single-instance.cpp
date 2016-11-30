#include <bits/stdc++.h>
#include "krazki.h"
#include "message.h"

using namespace std;

template<typename T>
class vector_from_one : public std::vector<T> {
    public:
        using std::vector<T>::vector;
        T& operator[] (size_t n) { return std::vector<T>::operator[](n-1); }
        const T& operator[] (size_t n) const { return std::vector<T>::operator[](n-1); }
        T& at (size_t n) { return std::vector<T>::at(n-1); }
        const T& at (size_t n) const { return std::vector<T>::at(n-1); }
};

void slave(int nodes, int node) {}

void master(int nodes, int node)
{
    int64_t holes_count = PipeHeight();
    int64_t discs = NumberOfDiscs();

    vector_from_one<int64_t> holes(holes_count);

    {
        int64_t width = numeric_limits<int64_t>::max();
        for(int64_t i = 1; i <= holes_count; ++i)
        {
            holes[i] = width = min<int64_t>(width, HoleDiameter(i));
        }
    }

    int64_t height = holes_count;
    int64_t answer;

    for(int64_t i = 1; i <= discs; ++i)
    {
        int64_t width = DiscDiameter(i);
        while(height > 0 and holes[height] < width)
            --height;

        if(height > 0)
        {
            //cout << "Disc "
            answer = height--;
        }
        else
            answer = 0;
    }

    cout << answer << endl;
}

int main() 
{
    int nodes, node;
    node = MyNodeId();
    nodes = NumberOfNodes();
    
    if( node == 0 )
        master(nodes, node);
    else
        slave(nodes, node);
}
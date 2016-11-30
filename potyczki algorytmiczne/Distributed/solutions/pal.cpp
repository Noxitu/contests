#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include "palindromy.h"
#include "message.h"

using namespace std;

class input_view 
{
    const size_t length;
public:
    class iterator
    {
        input_view const &input;
        size_t index;
    public:
        iterator(input_view const &input, size_t index) : input(input), index(index) {}
        char operator*() const { return input[index]; }
        void operator++() {++index;}
        friend bool operator==(iterator const &lhs, iterator const &rhs) { return lhs.index == rhs.index; }
        friend bool operator!=(iterator const &lhs, iterator const &rhs) { return lhs.index != rhs.index; }
        friend iterator operator+(iterator const &lhs, size_t plus) { return iterator(lhs.input, lhs.index + plus); }
        friend size_t operator-(iterator const &lhs, iterator const &rhs) { return lhs.index - rhs.index; }
    };
    input_view() : length(GetLength()) { }
    size_t size() const { return length; }
    char operator[](size_t i) const { return GetLetter(i); }
    iterator begin() const { return iterator(*this, 0); }
    iterator end() const { return iterator(*this, length); }
};

size_t get_right(pair<size_t, size_t> p) { return p.first; }
size_t get_middle(pair<size_t, size_t> p) { return p.second; }

template<typename stringlike, typename Operation>
void func(stringlike &input, Operation op)
{
    size_t length = 2*input.size()+1;
    auto haystack = [&input, length](size_t index)
    {
        return index == 0 ? '^' : index == length-1 ? '$' : index%2 == 0 ? '_' : input[index/2];
    };

    vector<size_t> max_radius(length, 0);

    pair<size_t, size_t> rightmost = {0, 0};

    for(size_t middle = 1; haystack(middle) != '$'; ++middle)
    {
        size_t &radius = max_radius[middle];

        if(get_right(rightmost) > middle)
        {
            size_t mirrored_position = 2*get_middle(rightmost) - middle;
            radius = min(get_right(rightmost) - middle, max_radius[mirrored_position]);
        }
        else
            radius = 0;

        while( haystack(middle-radius-1) == haystack(middle+radius+1) )
            ++radius;

        auto left = (middle-radius)/2;
        auto right = (middle+radius+1)/2;
        if(left != right)
            op(begin(input)+left, begin(input)+right);

        rightmost = max(rightmost, {middle+radius, middle});
    }
}

void slave(int nodes, int node) {}
void master(int nodes, int node)
{
    input_view input;

    size_t total = 0;
    func(input, [&total](input_view::iterator l, input_view::iterator r)
    {
        total += (r-l+1)/2;
    });

    cout << total << '\n';
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
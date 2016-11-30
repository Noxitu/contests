#include <vector>
#include <algorithm>
#include <iostream>
 
#include "krazki.h"
#include "message.h"

using namespace std;

void master(int nodes, int node)
{
    int64_t holes_count = PipeHeight();
    int64_t discs_count = NumberOfDiscs();

    for(int i = 0; i < nodes; ++i)
    {
        PutLL(i, i*holes_count/nodes+1);
        PutLL(i, (i+1)*holes_count/nodes+1);

        PutLL(i, i*discs_count/nodes+1);
        PutLL(i, (i+1)*discs_count/nodes+1);

        Send(i);
    }

    vector<int64_t> holes(nodes);
    vector<int64_t> discs(nodes);

    for(int i = 0; i < nodes; ++i)
    {
        int node = Receive(i);
        holes.at(node) = GetLL(node);
        discs.at(node) = GetLL(node);
    }

    partial_sum(begin(holes), end(holes), begin(holes), [](int64_t a, int64_t b){ return min(a, b); });
    partial_sum(begin(discs), end(discs), begin(discs), [](int64_t a, int64_t b){ return max(a, b); });

    //cout << holes << endl << discs << endl;
    //cerr << "Master enters phase #2." << endl;

    for(int h = nodes-1, d = 0, i = 0; h >= 0 and d < nodes; ++i)
    {
        PutLL(i/2, (h+1)*holes_count/nodes);
        PutLL(i/2, h*holes_count/nodes);
        PutLL(i/2, d*discs_count/nodes+1);
        PutLL(i/2, (d+1)*discs_count/nodes+1);

        if(i%2 == 1)
            Send(i/2);

        // can entire disc "block" fall to this hole "block"
        //cout << "Current hole (" << h << ") block can be entered by " << holes[h-1] << endl;
        //cout << "Current disc (" << d << ") block had " << discs[d] << endl;
        if( h == 0 ) ++d;
        else if( d == nodes-1 ) --h;
        else if( holes.at(h-1) >= discs.at(d) ) ++d;
        else --h;
    }
    PutLL(nodes-1, -1);
    Send(nodes-1);

    enum class JoinType { MoreDiscs, MoreHoles };
    JoinType current = JoinType::MoreDiscs;
    int64_t answer = holes_count+1;

    for(int h = nodes-1, d = 0, i = 0; h >= 0 and d < nodes; ++i)
    {
        if(i%2 == 0)
            Receive(i/2);

        int64_t count = (d+1)*discs_count/nodes - d*discs_count/nodes;
        int64_t local_answer = GetLL(i/2);

        switch(current)
        {
            case JoinType::MoreDiscs:
                answer = min(answer-count, local_answer);
                break;
            case JoinType::MoreHoles:
                answer = min(answer, local_answer);
                break;
            default: ;
        }

        //cout << "Updated:\n\tanswer = " << answer << "\n\tlocal_answer = " << local_answer << "\n\tfloors = " << h*holes_count/nodes+1 << " .. " << (h+1)*holes_count/nodes << "\n\tdiscs = " << d*discs_count/nodes+1 << " .. " << (d+1)*discs_count/nodes << endl;

        // can entire disc "block" fall to this hole "block"
        if( h == 0 ) { ++d; current = JoinType::MoreDiscs; }
        else if( d == nodes-1 ) { --h; current = JoinType::MoreHoles; }
        else if( holes.at(h-1) >= discs.at(d) ) { ++d; current = JoinType::MoreDiscs; }
        else { --h; current = JoinType::MoreHoles; }
    }

    //cerr << "Master ended." << endl;
    //cout << holes << endl << discs << endl;
    cout << max<int64_t>(0, answer) << endl;
}

void calculateHoles(int master_node)
{
    int64_t begin = GetLL(master_node);
    int64_t end = GetLL(master_node);

    int64_t width = numeric_limits<int64_t>::max()-1;
    for(int64_t y = begin; y != end; ++y)
    {
        width = min<int64_t>(width, HoleDiameter(y));
    }

    PutLL(master_node, width);
}

void calculateDiscs(int master_node)
{
    int64_t begin = GetLL(master_node);
    int64_t end = GetLL(master_node);

    int64_t width = 0;
    for(int64_t y = begin; y != end; ++y)
    {
        width = max<int64_t>(width, DiscDiameter(y));
    }

    PutLL(master_node, width);
}

void calculateMain(int master_node)
{
    int64_t hbegin = GetLL(master_node);
    if(hbegin == -1) return;
    int64_t hend = GetLL(master_node);
    int64_t dbegin = GetLL(master_node);
    int64_t dend = GetLL(master_node);

    //cout << "Calculating: [" << hbegin << ' ' << hend << ' ' << dbegin << ' ' << dend << "]" << endl;
    vector<int64_t> holes(hbegin-hend);
    for(int64_t h = hbegin, i = 0; h != hend; --h, ++i)
        holes.at(i) = HoleDiameter(h);

    partial_sum(holes.rbegin(), holes.rend(), holes.rbegin(), [](int64_t a, int64_t b){ return min(a, b); });

    int64_t current_level = 0;
    int64_t underflow = 0, count = 0, overflow = 0;
    int64_t answer = numeric_limits<int64_t>::max();
    for(int64_t d = dbegin; d != dend; ++d)
    {
        int width = DiscDiameter(d);
        while(current_level < (int64_t)holes.size() and holes.at(current_level) < width)
            ++current_level;

        if(current_level >= (int64_t) holes.size())
        {
            answer = hend-overflow;
            ++overflow;
            continue;
        } 

        if(current_level == 0)
        {
            ++underflow;
            continue;
        }

        ++count;
        answer = hbegin-current_level;
        ++current_level;
    }

    //cout << "Calculated: [" << hbegin << ' ' << hend << ' ' << dbegin << ' ' << dend << "]: " << holes << endl;
    //cout << "Calculated: [" << hbegin << ' ' << hend << ' ' << dbegin << ' ' << dend << "]: answer = " << answer << "\n\tunderflow = " << underflow << "  count = " << count << "  overflow = " << overflow << "  lvl = " << hbegin-current_level << endl;

    //PutLL(master_node, underflow);
    //PutLL(master_node, count);
    //PutLL(master_node, overflow);
    PutLL(master_node, answer);
}

void slave(int master_node, int node)
{
    Receive(master_node);
    calculateHoles(master_node);
    calculateDiscs(master_node);
    Send(master_node);

    Receive(master_node);
    calculateMain(master_node);
    calculateMain(master_node);
    Send(master_node);

    //cerr << "Slave #" << node << " ended." << endl;
}

int main() 
{
    int nodes, node;
    node = MyNodeId();
    nodes = NumberOfNodes();
    nodes = min(nodes, PipeHeight()+1);
    
    if(node >= nodes)
        return 0;

    if( node == nodes-1 )
        master(nodes-1, node);
    else
        slave(nodes-1, node);
    return 0;
}

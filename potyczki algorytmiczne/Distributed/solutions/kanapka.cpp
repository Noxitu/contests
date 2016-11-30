#include <iostream>
#include <algorithm>
#include "kanapka.h"
#include "message.h"

using namespace std;

void slave(int nodes, int node) {
    int n = GetN();
    int begin = 1LL*(node-1)*n/(nodes-1);
    int end = 1LL*node*n/(nodes-1);
    
    long long sum = 0, prefix = 0, infix = 0, tmp = 0;
    for( int i = begin; i < end; i++ ) {
        long long taste = GetTaste(i);
        //std::cerr << "taste(" << i << ") = " << taste << std::endl;
        sum += taste;
        prefix = min( prefix, sum );
        
        tmp += taste;
        tmp = min( tmp, 0LL );
        infix = min( infix, tmp );
    }
    
    PutLL( 0, prefix );
    PutLL( 0, sum );
    PutLL( 0, infix );
    
    long long sufix = sum;
    for( int i = begin; i < end; i++ ) {
        sum -= GetTaste(i);
        sufix = min( sufix, sum );
    }
    
    PutLL( 0, sufix );
    //std::cerr << node << " = " << prefix << ' ' << sum << ' ' << infix << ' ' << sufix << std::endl;
    Send( 0 );
}

void master(int nodes, int node) {
    long long answer = 0, sufix = 0, sum = 0;
    for( int i = 1; i < nodes; i++ ) {
        Receive( i );
        long long partial_prefix = GetLL( i );
        long long partial_sum = GetLL( i );
        long long partial_infix = GetLL( i );
        long long partial_sufix = GetLL( i );
        
        sum += partial_sum;
        answer = min( answer, partial_infix );
        answer = min( answer, sufix+partial_prefix );
        sufix = min( sufix+partial_sum, partial_sufix );
    }
    
    cout << sum-answer << endl;
}

int main() {
    int nodes, node;
    node = MyNodeId();
    nodes = NumberOfNodes();
    
    if( node == 0 )
        master(nodes, node);
    else
        slave(nodes, node);
        
    return 0;
}

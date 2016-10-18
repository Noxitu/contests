#include <iostream>
#include <algorithm>

using namespace std;

char T[1000000];

void test() {
    cin >> T;
    int sum=0, up = 0, down = T[1] == 0 ? 0 : -1;
    for( char *c = T; *c; c++ ) {
        sum += *c - '0';
        up += '9'-*c;
        down += *c - '0';
    }
    sum %= 9;
    int xsum = (9-sum)%9;
    if( ( xsum < sum and xsum <= up ) or sum > down )
        sum = xsum;
    cout << sum << '\n';
}

int main() {
    int T;
    cin >> T;
    while( T --> 0 )
        test();
}
#include <iostream>
 
using namespace std;
 
inline int abs( int a ) { return a > 0 ? a : -a; }
 
void test() {
    int n, ax, ay, bx, by, cx, cy;
    cin >> n >> ax >> ay >> cx >> cy >> bx >> by;
    
    if( ax == cx and ax == bx and ((ay<by) == (by<cy)) ) {
        cout << abs(cy-ay)+2 << '\n';
    } else if( ay == cy and ay == by and ((ax<bx) == (bx<cx)) ) {
        cout << abs(cx-ax)+2 << '\n';
    } else {
        cout << abs(cx-ax)+abs(cy-ay) << '\n';
    }
}

 
int main() {
    ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    while( T --> 0 )
        test();

}
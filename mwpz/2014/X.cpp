#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>

using namespace std;

struct test_t {
  int generator, question, i, answer;
};

bool by_generator(const test_t& a, const test_t& b) { return a.generator < b.generator; }
bool by_i(const test_t& a, const test_t& b) { return a.i < b.i; }

int main() {
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  vector<test_t> Q(T);
  for( int i = 0; i < T; i++ ) {
    Q[i].i = i;
    string bigint;
    cin >> Q[i].question >> bigint;
    bigint = bigint.substr(max(bigint.size()-3,0LU));
    Q[i].generator = atoi(bigint.c_str());
  }
  
  sort( Q.begin(), Q.end(), by_generator );
  
  for( int i = 0; i < T; ) {
    int val = 1, generator = Q[i].generator;
    int logs[1000];
    for( int j = 0; j < 1000; j++ )
      logs[j] = -1;
    
    for( int power = 0;; power++ ) {
      val *= generator;
      val %= 1000;
      if( logs[val] != -1 )
        break;
      logs[val] = power;
    }
    
    for(; i < T and Q[i].generator == generator; i++ )
      Q[i].answer = logs[Q[i].question];
  }
  
  sort( Q.begin(), Q.end(), by_i );
  
  for( int i = 0; i < T; i++ )
    if( Q[i].answer == -1 )
      cout << "NIE\n";
    else
      cout << Q[i].answer << '\n';
   
    
  return 0;
}

#include <bits/stdc++.h>
#include <stdint.h>

#include "krazki.h"
#include "message.h"

using namespace std;


void slave() {
  int n = NumberOfNodes();
  int ii = MyNodeId();
  
  int s = n - 1;
  int h = PipeHeight();
  s = min(s, h);
  
  if(ii >= s) return;
  
  int i0 = 1 + h * ii       / s;
  int i1 =     h * (ii + 1) / s;
  
  vector<int64_t> pipe(i1 - i0 + 1);
  
  for(int i = i0; i <= i1; i++) pipe.at(i - i0) = HoleDiameter(i);
  
  for(int i = i0 + 1; i <= i1; i++) pipe.at(i - i0) = min(pipe.at(i - i0), pipe.at(i - i0 - 1));
  
  PutLL(n-1, pipe.at(0));
  Send(n-1);
  // cerr << "send " << ii << " -> " << n-1 << '\n';
  
  Receive(n-1);
  int64_t prev = GetLL(n-1);
  
  for(int i = i0; i <= i1; i++) pipe.at(i - i0) = min(prev, pipe.at(i - i0));
  
  int k = NumberOfDiscs();
  
  int64_t iz = i1;
  
  while(true) {
    int src = Receive(-1);
    int j = GetInt(src);
    
    if(j == -1) return;
    
    while(j <= k) {
      int64_t d = DiscDiameter(j);
      // cerr << "ii = " << ii << ", Disc #" << j << " (" << d << ")\n";
      
      while(iz >= i0 and d > pipe.at(iz - i0)) iz--;
      
      if(iz >= i0) {
        if(j == k) {
          cout << max<int64_t>(iz, 0) << '\n';
          PutInt(n-1, -1);
          Send(n-1);
          // cerr << "send " << ii << " -> " << n-1 << '\n';
          break;
        }
      } else {
        if(ii == 0) {
          cout << "0\n";
          PutInt(n-1, -1);
          Send(n-1);
          // cerr << "send " << ii << " -> " << n-1 << '\n';
          break;
        } else {
          PutInt(ii-1, j);
          Send(ii-1);
          // cerr << "send " << ii << " -> " << ii-1 << '\n';
          break;
        }
      }
      
      iz--;
      j++;
    }
  }
}


void killSlaves() {
  int n = NumberOfNodes();
  int s = n - 1;
  int h = PipeHeight();
  s = min(s, h);
  
  for(int i = 0; i < s; i++) {
    PutInt(i, -1);
    Send(i);
  }
}


void master() {
  int n = NumberOfNodes();
  int s = n - 1;
  int h = PipeHeight();
  s = min(s, h);
  
  vector<int> tops;
  for(int i = 0; i < s; i++) {
    Receive(i);
    int64_t top = GetLL(i);
    tops.push_back(top);
    if(i != 0) tops.at(i) = min(tops.at(i), tops.at(i-1));
  }
  
  for(int i = 0; i < s; i++) {
    PutLL(i, i == 0 ? numeric_limits<int64_t>::max() : tops.at(i-1));
    Send(i);
  }
  
  int z = tops.size() - 1;
  PutInt(z, 1);
  Send(z);
  
  int src = Receive(-1);
  int j = GetInt(src);
  
  if(j == -1) killSlaves();
}


int main() { 
  int n = NumberOfNodes();
  
  if(MyNodeId() != n-1) {
    slave();
  } else {
    master();
  }
  
  return 0;
}
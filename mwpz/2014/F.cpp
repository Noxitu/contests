#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct question {
	int x, y, k;
	int a, id;
};

int n, m, z;

istream& operator>>(istream& in, question& q) {
	in >> q.x >> q.y >> q.k;
	q.x = min(q.x, n+1-q.x);
	q.y = min(q.y, m+1-q.y);
	return in;
}

bool comp1( const question a, const question b ) {
	if( a.x != b.x )
		return a.x < b.x;
	return a.y < b.y;
}

bool comp2( const question a, const question b ) {
	return a.id < b.id;
}


int D[101][101];

inline void use_edge( queue<pair<int, int> >& queue, int px, int py, int D[101][101], int dx, int dy ) {
	if( px+dx >= 1 and px+dx <= n and py+dy >= 1 and py+dy <= m and D[px+dx][py+dy] == 2000 ) {
		queue.push( make_pair( px+dx, py+dy ) );
		D[px+dx][py+dy] = D[px][py] + 1;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin >> n >> m >> z;
	
	vector<question> Q(z);
	for( int i = 0; i < z; i++ ) {
		cin >> Q[i];
		Q[i].id = i;
	}
		
	sort( Q.begin(), Q.end(), comp1 );
	
	for( int zid = 0; zid < z; ) {
		int qx = Q[zid].x, qy = Q[zid].y;
	
		for( int x = 1; x <= n; x++ )
			for( int y = 1; y <= m; y++ )
				D[x][y] = 2000;
				
		queue<pair<int, int> > queue;
		queue.push( make_pair(qx,qy) );
		D[qx][qy] = 0;
		while( not queue.empty() ) {
			int px = queue.front().first;
			int py = queue.front().second;
			queue.pop();
			
			use_edge( queue, px, py, D, -2, -1 );
			use_edge( queue, px, py, D, -2, 1 );
			use_edge( queue, px, py, D, -1, -2 );
			use_edge( queue, px, py, D, -1, 2 );
			use_edge( queue, px, py, D, 1, -2 );
			use_edge( queue, px, py, D, 1, 2 );
			use_edge( queue, px, py, D, 2, -1 );
			use_edge( queue, px, py, D, 2, 1 );
		}
		
		int fields[2001];
		for( int i = 0; i <= 1000; i++ )
			fields[i] = 0;
		
		for( int x = 1; x <= n; x++ )
			for( int y = 1; y <= m; y++ )
				fields[D[x][y]]++;
				
		for( int k = 2; k <= 1000; k++ )
			fields[k] += fields[k-2];
			
		for( ; zid < z and Q[zid].x == qx and Q[zid].y == qy; zid++ )
			Q[zid].a = fields[Q[zid].k];
	}
	
	sort( Q.begin(), Q.end(), comp2 );
	
	
	
	for( int zid = 0; zid < z; zid++ )
		cout << Q[zid].a << '\n';
}
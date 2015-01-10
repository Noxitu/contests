#include <cstdio>
#include <set>
#include <queue>

struct pos_t {
	int i,x,y;
	bool operator< (const pos_t& b) const {
		return i < b.i;
	} 
};
bool V[200001];

bool byX( const pos_t& a, const pos_t& b ) {
	return a.x < b.x or (a.x == b.x and a.y < b.y);
}

bool byY( const pos_t& a, const pos_t& b ) {
	return a.y < b.y or (a.y == b.y and a.x < b.x);
}

std::set<pos_t,bool(*)(const pos_t&,const pos_t&)> X(byX);
std::set<pos_t,bool(*)(const pos_t&,const pos_t&)> Y(byY);

int main() {
	int n;
	scanf("%d", &n );

	std::priority_queue<std::pair<int, pos_t> > Q;

	for( int i = 1; i <= n; i++ ) {
		pos_t p;
		p.i = i;
		V[i] = false;
		scanf("%d %d", &p.x, &p.y );
		if( i == 1 ) Q.push(std::make_pair(0, p));
		
		X.insert(p);
		Y.insert(p);
	}
	
	while( not Q.empty() ) {
		int d = -Q.top().first;
		pos_t p = Q.top().second;
		Q.pop();
		
		if( V[p.i] )
			continue;
			
		V[p.i] = true;
		
		if(p.i == n) {
			printf("%d\n", d);
			break;
		}
		
		std::set<pos_t,bool(*)(const pos_t&,const pos_t&)>::iterator it, jt;
		
		it = jt = X.find(p);
		it--; jt++;
		if( it != X.end() ) Q.push(std::make_pair(-d-(p.x - it->x), *it));
		if( jt != X.end() ) Q.push(std::make_pair(-d-(jt->x - p.x), *jt));
		
		it = jt = Y.find(p);
		it--; jt++;
		if( it != Y.end() ) Q.push(std::make_pair(-d-(p.y - it->y), *it));
		if( jt != Y.end() ) Q.push(std::make_pair(-d-(jt->y - p.y), *jt));
		
	}
	
	return 0;
}
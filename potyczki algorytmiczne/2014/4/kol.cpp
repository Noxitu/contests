#include <cstdio>
#include <cstdlib>
#include <message.h>
#include <climits>
#include <algorithm>
#include "kollib.h"
#include <set>
#include <map>
#include <vector>

struct important_t {
	int v, d;
	important_t(int v, int d) : v(v), d(d) {}
};
struct line_t {
	int starter;
	int direction;
	int previous;
	int ending;
	int length;

	void print() {
		printf("    [%d] [%d] ---(%d)--> [%d] [%d]\n    ", starter, direction, length, previous, ending);
		for( int i = 0; i < important.size(); i++ )
			printf("(%d, %d) ", important[i].v, important[i].d);
		printf("\n");
	}

	std::vector<important_t> important;

};


std::set<int> important;

int Amount[100];

bool comp(const line_t& a, const line_t& b) { return a.starter < b.starter; }


int main() {
	int I = MyNodeId(), M = NumberOfNodes();
	int N = NumberOfStudents();
	int PerNode = std::min(N/M, 250000);
	int TotalStarters = PerNode * M;
	line_t* lines = new line_t[PerNode*2];

	int K = NumberOfQueries();
	for( int i = 1; i <= K; i++ ) {
		important.insert(QueryFrom(i));
		important.insert(QueryTo(i));
	}

	// Inicjacja każdej linii
	for( int i = 0; i < PerNode; i++ ) {
		int s = I+M*i+1;
		lines[2*i].length = lines[2*i+1].length = 2;
		lines[2*i].starter = lines[2*i+1].starter = s;
		lines[2*i].previous = lines[2*i+1].previous = s;
		lines[2*i].direction = lines[2*i].ending = FirstNeighbor(s);
		lines[2*i+1].direction = lines[2*i+1].ending = SecondNeighbor(s);
	}

	// Rozrost każdej linii:
	for( int i = 0; i < PerNode*2; i++ ) {
		line_t& l = lines[i];
		while( l.ending > TotalStarters ) {
			int s = FirstNeighbor(l.ending);
			if( s == l.previous ) s = SecondNeighbor(l.ending);
			
			if( important.count(l.ending) == 1 )
				l.important.push_back( important_t( l.ending, l.length-1 ) );

			l.previous = l.ending;
			l.ending = s;
			l.length++;
		}

	}

	// Rozrost linii w oparciu o inne instancje
	for( int i = 0; i < I; i++ ) {
		Receive(i);
		//printf("received from %d\n", i );
		int n = GetInt(i);
		while( n --> 0 ) {
			int s = GetInt(i);
			int j = (s-1)/M*2;
			if( lines[j].direction != GetInt(i) )
				j++;

			lines[j].length = GetInt(i);
			lines[j].previous = GetInt(i);
			lines[j].ending = GetInt(i);
			int m = GetInt(i);
			lines[j].important.clear();
			while( m --> 0 ) {
				int v = GetInt(i);
				int d = GetInt(i);
				lines[j].important.push_back(important_t(v,d));
			}
		}
	}

	// Laczenie linii o takich samych koncach
	for( int i = 0; i < PerNode*2; i++ ) {
		line_t& l = lines[i];
		if( l.starter == INT_MAX )
			continue;

		while( l.ending % M == I+1 ) {
			int j = (l.ending-1) / M * 2;
			lines[j].starter = INT_MAX;
			lines[j+1].starter = INT_MAX;
			if( l.previous == lines[j].direction )
				j++;


			if( important.count(l.ending) == 1 )
				l.important.push_back( important_t( l.ending, l.length-1 ) );

			for( int k = 0; k < lines[j].important.size(); k++ )
				l.important.push_back(important_t(lines[j].important[k].v, lines[j].important[k].d+l.length-1));
			
			l.length += lines[j].length - 1;
			l.previous = lines[j].previous;
			l.ending = lines[j].ending;
		}
	}


	// Laczenie linii w puncie startu
	for( int i = 0; i < PerNode; i++ ) {
		line_t &a = lines[2*i], &b = lines[2*i+1];
		if( a.starter == INT_MAX )
			continue;

		if( (a.ending-1)%M < (b.ending-1)%M )
			std::swap(a,b);

		if( (a.ending-1)%M == (b.ending-1)%M and a.ending < b.ending )
			std::swap(a,b);

		for( int k = 0; k < a.important.size(); k++ )
			a.important[k].d += b.length-1;

		for( int k = 0; k < b.important.size(); k++ )
			a.important.push_back(important_t(b.important[k].v, b.length-1-b.important[k].d));

		if( important.count(a.starter) == 1 )
			a.important.push_back( important_t( a.starter, b.length-1 ) );

		a.starter = b.ending;
		a.direction = b.previous;
		a.length += b.length-1;
		b.starter = INT_MAX;
	}

	std::sort(lines, lines+2*PerNode, comp );


	for( int i = 0; i < 2*PerNode and lines[i].starter != INT_MAX; i++ ) {
		lines[i].print();
	}//*/

	for( int i = 0; i < 2*PerNode and lines[i].starter != INT_MAX; i++ ) {
		int node = (lines[i].starter-1)%M;
		Amount[node]++;
	}

	for( int i = I+1; i < M; i++ ) {
		PutInt(i, Amount[i] );
	}

	for( int i = 0; i < 2*PerNode and lines[i].starter != INT_MAX; i++ ) {
		line_t &l = lines[i];
		int node = (l.starter-1)%M;
		PutInt(node, l.starter);
		PutInt(node, l.direction);
		PutInt(node, l.length);
		PutInt(node, l.previous);
		PutInt(node, l.ending);
		PutInt(node, l.important.size());
		for( int i = 0; i < l.important.size(); i++ ) {
			PutInt(node, l.important[i].v);
			PutInt(node, l.important[i].d);
		}
	}

	for( int i = I+1; i < M; i++ ) {
		Send(i);
		//printf("send to %d\n", i );
	}

	if( I+1 == M ) {
		std::map<int, int> Answer;
		int j = 0;
		while( lines[j].length < N ) j++;

		for( int i = 0; i < lines[j].important.size(); i++ )
			Answer[lines[j].important[i].v] = lines[j].important[i].d;

		for( int i = 1; i <= K; i++ ) {
			int a = QueryFrom(i);
			int b = QueryTo(i);
			int d = (Answer[a]-Answer[b]+N)%N;
			d = std::min(d, N-d); 
			printf("%d\n", d);
		}
	}
	
}

#include <cstdio>
#include <cstdlib>
#include <message.h>
#include <climits>
#include <algorithm>
#include "kollib.h"
#include <set>
#include <map>
#include <vector>

int Q[400];
std::set<int> SX;
int X[100000];
int V[100000];
int D[100000];
std::multimap<int, std::pair<int, int> > Map;
std::map<int, int> Pos;

unsigned short lfsr = 0xACE1u;

// Just for sure
unsigned my_rand()
{
	unsigned register bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
	return lfsr =  (lfsr >> 1) | (bit << 15);
}

int main() {
	int I = MyNodeId(), M = NumberOfNodes();
	int N = NumberOfStudents();
	int K = NumberOfQueries();

	if( M%2 == 0 ) M--;
	M--;

	if( I > M ) {
		//puts("I am useless. :-(");
		return 0;
	}
	if( I == M ) {
		//puts("I am server. :-)");
	}

	for( int i = 1; i <= K; i++ ) {
		SX.insert(QueryFrom(i));
		SX.insert(QueryTo(i));
	}

	for( int i = 0; i < M*400; i++ ) {
		SX.insert( ( 1LL*my_rand()*RAND_MAX + my_rand() ) % N + 1 );
	}

	int LX = 0;
	for( std::set<int>::iterator it = SX.begin(); it != SX.end() and 2*LX/M < 960; it++ ) {
		X[LX++] = *it;
	}

	if( I < M )
		for( int i = I/2; i < LX; i += M/2 ) {
			int from = X[i];
			int previous = from;
			int x = (I%2 == 0) ? FirstNeighbor(from) : SecondNeighbor(from);
			int distance = 1;
			while( SX.count(x) == 0 ) {
				int y = FirstNeighbor(x);
				if( y == previous )
					y = SecondNeighbor(x);
				previous = x;
				x = y;
				distance++;
			}
			//printf("calculated[%d] %d->%d\n", i, from, x );
			PutInt(M, from);
			PutInt(M, x);
			PutInt(M, distance);
			Send(M);
			//puts("sending");
		}		
	
	if( I == M ) {
		int n = 1;
		V[0] = X[0];
		D[0] = 0;

		int total = LX;
		while( n < total ) {
			int i = Receive(-1);
			int a = GetInt(i), b = GetInt(i);
			int dist = GetInt(i);
			//printf("recieving(%d): %d %d %d\n", i, a, b, dist );
			Map.insert( std::make_pair( a, std::make_pair(b, dist) ) );
			Map.insert( std::make_pair( b, std::make_pair(a, dist) ) );

			while( n < total and Map.count(V[n-1]) > 0 ) {
				a = V[n-1];
				std::multimap<int, std::pair<int, int> >::iterator it = Map.find(a);
				b = it->second.first;
				dist = it->second.second;
				if( n == 1 or V[n-2] != b ) {
					//printf("Adding %d after %d\n", b, a );
					V[n] = b;
					D[n] = D[n-1]+dist;
					n++;
				}
				Map.erase(it);
			}
		}
		for( int i = 0; i < total; i++ ) {
			Pos[V[i]] = D[i];
		}

		for( int i = 1; i <= K; i++ ) {
			int a = QueryFrom(i), b = QueryTo(i);
			int d = (Pos[a] - Pos[b] + N)%N;
			d = std::min(d, N-d);
			printf("%d\n", d);
		}
	}
}

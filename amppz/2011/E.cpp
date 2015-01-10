#include <cstdio>
#include <climits>
#include <cmath>
#include <set>
#include <map>

std::map< int, int > N;  	// Norki w postaci par (pozycja, nr. porządkowy)
std::set< int > M;			// Pozycje magnetofonów

// Funkcja zwraca unikalny, czyli nienależący do innego magnetofonu zakres
// Nominalny zakres magnetofonu to <position-l, position+l>
// Dolna granica musi być większa, niż górna granica magnetofonu stojącego po lewej
// Górna granica musi być mniejsza, niż dolna granica magnetofonu stojącego po prawej
std::pair< int, int > get_unique_range( std::set<int>::iterator ileft, int l ) {
	int position = *ileft;
	std::set<int>::iterator iright = ileft;
	ileft--; iright++;
	int left = std::max( position-l, *ileft+l+1 );
	int right = std::min( position+l, *iright-l-1 );	
	return std::make_pair( left, right );
}

// Funkcja liczy norki w zadanym zakresie
// Znajdujemy pierwszy magnetofon należący do zakresu, czyli o pozycji nie mniejszej niż dolna granica
// Znajdujemy też pierwszy magnetofon za zakresem, czyli o pozycji większej niż górna granica
// Różnica nr. pożądkowych to ilość w zakresie
int get_count(std::pair<int, int> range ) {
	if( range.first > range.second )
		return 0;
	int left = N.lower_bound(range.first)->second;
	int right = N.upper_bound(range.second)->second;
	return right-left;
}

int main() {
	int n, k, d, l;

	scanf("%d %d %d %d", &n, &k, &d, &l );

	N[INT_MIN] = -1;
	N[0] = 0;
	N[INT_MAX] = n;
	for( int i = 1; i < n; i++ ){
		int val;
		scanf("%d", &val );
		N[val] = i;
	}

	M.insert(INT_MIN);
	M.insert(INT_MAX);

	int bad = 0;
	while( k --> 0 ) {
		int val;
		scanf("%d", &val );
		bad += get_count( get_unique_range( M.insert(val).first, l ) );
	}
	
	printf("%d\n", bad );
	while( d --> 0 ) {
		int from, to;
		scanf("%d %d", &from, &to );
		std::set<int>::iterator it = M.find(from);
		bad -= get_count( get_unique_range( it, l ) );
		M.erase(it);
		it = M.insert(to).first;
		bad += get_count( get_unique_range( it, l ) );
		printf("%d\n", bad );
	}
}

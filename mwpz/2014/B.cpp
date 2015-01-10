#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

using namespace std;

struct partia_t;
struct kandydat_t;
unsigned int glosy = 0;

struct partia_t {
	vector<kandydat_t*> kandydaci;
	int numer;
	string nazwa;
	unsigned int glosy;
	unsigned int mandaty;
	
	partia_t(int numer) : numer(numer), glosy(0), mandaty(0) {}
	
	void dodaj_mandat();
};

struct kandydat_t {
	partia_t* partia;
	int numer;
	string imie;
	string nazwisko;
	unsigned int glosy;
	bool mandat;
	
	kandydat_t(int numer) : numer(numer), glosy(0), mandat(false) {}
	
	void dodaj_glos() {
		glosy++;
		partia->glosy++;
		::glosy++;
	}
};

void partia_t::dodaj_mandat() {
	kandydaci[mandaty]->mandat = true;
	mandaty++;
}

bool ponizej_progu(const partia_t* P) {
	return 20*P->glosy <= glosy;
}

bool wszyscy_maja_mandaty(const partia_t* P) {
	return P->mandaty == P->kandydaci.size();
}

bool wiecej_glosow(const kandydat_t* a, const kandydat_t* b) {
	if( a->glosy != b->glosy )
		return a->glosy > b->glosy;
	return a->numer < b->numer;
}

bool pierszenstwo_do_mandatu(const partia_t* a, const partia_t* b) {
	if( a->glosy*(b->mandaty+1) != b->glosy*(a->mandaty+1) )
		return a->glosy*(b->mandaty+1) > b->glosy*(a->mandaty+1);
		
	if( a->glosy != b->glosy )
		return a->glosy > b->glosy;
		
	if( a->kandydaci[a->mandaty]->glosy != b->kandydaci[b->mandaty]->glosy )
		return a->kandydaci[a->mandaty]->glosy > b->kandydaci[b->mandaty]->glosy;
		
	return a->numer > b->numer;
}

int main() {
	ios_base::sync_with_stdio(false);
	
	int p;
	cin >> p;
	
	vector<partia_t*> partie(p);
	for( int i = 0; i < p; i++ ) {
		partia_t* P = partie[i] = new partia_t(i+1);
		cin >> P->nazwa;
	}
	
	int k;
	cin >> k;
	
	vector<kandydat_t*> kandydaci(k);
	for( int i = 0; i < k; i++ ) {
		kandydat_t* K = kandydaci[i] = new kandydat_t(i+1);
		
		int partia;
		cin >> partia >> K->imie >> K->nazwisko;
		K->partia = partie[partia-1];
		K->partia->kandydaci.push_back(K);
	}
	
	int g;
	cin >> g;
	for( int i = 0; i < g; i++ ) {
		int glos;
		cin >> glos;
		if( 1 <= glos and glos <= k ) {
			kandydaci[glos-1]->dodaj_glos();
		}
	}
	
	vector<partia_t*>::iterator partie_end = remove_if( partie.begin(), partie.end(), ponizej_progu );
	for( vector<partia_t*>::iterator it = partie.begin(); it != partie_end; it++ )
		sort( (*it)->kandydaci.begin(), (*it)->kandydaci.end(), wiecej_glosow );
	
	int mandaty;
	cin >> mandaty;
	while( mandaty --> 0 ) {
		partie_end = remove_if( partie.begin(), partie_end, wszyscy_maja_mandaty );
		
		partia_t* mandat_dla = *min_element( partie.begin(), partie_end, pierszenstwo_do_mandatu );
		
		mandat_dla->dodaj_mandat();
	}
	
	for( vector<kandydat_t*>::iterator it = kandydaci.begin(); it != kandydaci.end(); it++ )
		if( (*it)->mandat )
			cout << (*it)->imie << ' ' << (*it)->nazwisko << '\n';
	
	return EXIT_SUCCESS;
}

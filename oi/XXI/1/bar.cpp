#include <iostream>
#include <string>
#include <stack>

using namespace std;

struct Salad {
	int top, bottom, begin;
};

int main() {
    ios_base::sync_with_stdio(false);
    
    int n;
    static char fruits[1000001];
    cin >> n >> fruits;
    
	int level = 0, answer = 0;
	stack<Salad> previous;
	Salad empty = {0,0,0};
	previous.push(empty);
    
	for( int position = 0; position < n; position++ ) {
		if( fruits[position] == 'p' )
            level++;
		else
            level--;

		Salad salad = {level, level, position};

		while( not previous.empty() and level >= previous.top().top ) {
			salad.begin = previous.top().begin;
			salad.bottom = previous.top().bottom;
			stack.pop();
		}

		while( not previous.empty() and level < previous.top().bottom ) {
			previous.pop();
		}

		if( position - salad.begin > answer )
			answer = position - salad.begin;

		previous.push(salad);
	}
	cout << answer << '\n';
}
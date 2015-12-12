#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    string code;
    getline( cin, code, '\0' );
    // remove strings
    {
        bool isString = false;
        bool wasBackslash = false;
        auto it = remove_if( begin(code), end(code), [&](char c)->bool{
            bool wasString = isString;
            isString ^= (c=='"')*!wasBackslash;
            wasBackslash = (c=='\\')*!wasBackslash;
            return isString*wasString;
        });
        code.erase( it, end(code) );
    }
    
    transform( begin(code), end(code), begin(code), [](char c) {
        return isalnum(c) or c == '_' ? c : ' ';
    });
    
    // remove multispaces
    {
        bool wasSpace = true;
        auto it = remove_if( begin(code), end(code), [&](char c)->bool{
            bool ignore = wasSpace*(c==' ');
            wasSpace = (c==' ');
            return ignore;
        });
        code.erase( it, end(code) );
    }
    
    stringstream scode(code);
    multiset<string> words;    
    for( string word; scode >> word; ) {
        words.insert(word);
    }
    
    int bad = words.count("for")+words.count("while")+words.count("if")+words.count("switch");
    cout << vector<string>{"POPRAWNY", "UJDZIE", "MASAKRA"}[min(2, bad/10)] << endl << bad+1 << endl;
}


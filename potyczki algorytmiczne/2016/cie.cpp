#include "cielib.h"
#include <bits/stdc++.h>

using namespace std;

int get_sigma(int a, int b)
{
    const int len = b-a;
    if( len == 1 )
        return 0;
    return (len+1)/2;
}

pair<int, int> get_guesses(int a, int b)
{
    if(a+1 != b)
        return {b, a};

    if(a == 0)
        return {b+1, a};
    else
        return {a-1, b};
}
void update_corners(int &a, int &b, const int answer, const int sigma)
{
    if(sigma > 0)
    {
        if(answer == 1)
            b = a+sigma;
        else
            a = b-sigma;
    }
    else
    {
        if( a==0 and answer == 1 or a != 0 and answer == 0 )
            b = a;
        else
            a = b;
    }
}

int main()
{
    int d = podajD();
    int r = podajR();

    vector<int> corner1(d, 0);
    vector<int> corner2(d, r);

    while(corner1.front() != corner2.front())
    {
        vector<int> pnt(d);
        for(int i = 0; i < d; ++i)
            pnt[i] = (corner1[i]+corner2[i])/2;

        const int sigma = get_sigma(corner1[0], corner2[0]);

        for(int i = 0; i < d; ++i)
        {
            auto guess = get_guesses(corner1[i], corner2[i]);

            pnt[i] = guess.first;
            czyCieplo(pnt.data());

            pnt[i] = guess.second;
            int answer = czyCieplo(pnt.data());
            pnt[i] = (corner1[i]+corner2[i])/2;

            update_corners(corner1[i], corner2[i], answer, sigma);
        }
    }

    znalazlem(corner1.data());
}
    

// Not solved!

#include <bits/stdc++.h>
// #define MULTIPLE_TESTS
// #define ENDLESS_TESTS

using namespace std;

struct Entry
{
    int ways;
    int errors;
};

class Value
{
private:
public:
    //vector<
};

class Matrix 
{
private:
    int m_n;
    vector<int> m_data;

public:
    Matrix(int n) : m_n(n), m_data(n*n, 0) {}

    int n() const { return m_n; }
    int& operator()(int y, int x) { return m_data[y*m_n+x]; }
    const int& operator()(int y, int x) const { return m_data[y*m_n+x]; }

    Matrix upper() const
    {
        Matrix ret(m_n);

        for (int y = 0; y < m_n; ++y)
            for (int x = y+1; x < m_n; ++x)
                ret(y, x) = (*this)(y, x);

        return ret;
    }
};

Matrix operator* (const Matrix &lhs, const Matrix &rhs)
{
    assert(lhs.n() == rhs.n());

    const int n = lhs.n();
    Matrix ret(n);

    for (int y = 0; y < n; ++y)
        for (int x = 0; x < n; ++x)
            for (int k = 0; k < n; ++k)
                ret(y, x) += lhs(y, k) * rhs(k, x);

    return ret;
}

ostream& operator<<(ostream &out, const Matrix &mat)
{
    for (int y = 0; y < mat.n(); ++y)
    {
        for (int x = 0; x < mat.n(); ++x)
            out << mat(y, x) << ' ';
        out << '\n';
    }
    return out;
}

void test()
{
    int n;
    cin >> n;

    vector<int> data(n);
    for (auto &val : data)
        cin >> val;

    const int source = 0;
    const int sink = n+1;

    Matrix mat(n+2), wrong(n+2);
    mat(source, sink) = 1;

    for (int i = 1; i <= n; ++i)
        mat(source, i) = mat(i, sink) = 1;

    for (int i = 0; i < n; ++i)
        for (int j = i+1; j < n; ++j)
            if (data[i] < data[j])
                mat(data[i], data[j]) = 1;
            else
                wrong(data[i], data[j]) = 1;

    auto total = mat;
    int k = 0;
    while (total(source, sink))
    {
        cout << "k = " << k++ << endl;
        cout << total(source, sink) << endl;
        total = total * mat;
    }
}


int main()
{
#ifndef CONTEST_WORKSPACE
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
#endif

#ifdef ENDLESS_TESTS
    while(!(cin >> std::ws).eof())
        test();
#else
    int T = 0;

#ifdef MULTIPLE_TESTS
    cin >> T;
#else
    T = 1;
#endif

    while (T --> 0)
        test();
#endif

    return 0;
}
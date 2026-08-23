#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"
#include <bits/stdc++.h>
using namespace std;
#include "convolution/ntt.hpp"
#include "mod/modint.hpp"

int main() {
        using Mint = ModInt<998244353>;
        vector<Mint> a = {1, 2, 3};
        vector<Mint> b = {4, 5};
        auto c = convolution(a, b);
        assert(c.size() == 4);
        assert(c[0].value() == 4);
        assert(c[1].value() == 13);
        assert(c[2].value() == 22);
        assert(c[3].value() == 15);
        cout << "Hello World" << endl;
        return 0;
}

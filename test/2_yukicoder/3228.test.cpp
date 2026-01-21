#define PROBLEM "https://yukicoder.me/problems/no/3228"
#include "ds/matrix.hpp"
#include "mod/modint.hpp"
#include <bits/stdc++.h>
using namespace std;

void solve() {
        int64_t a, b, c, d, e, N;
        cin >> a >> b >> c >> d >> e >> N;
        using Mint = ModInt<1000000007>;
        if (N < 2) {
                Mint out = (N ? a + b : a);
                cout << out << '\n';
                return;
        }
        Matrix<Mint> transition(4, 4), base(4, 1);
        transition.table = {{c, d, 0, e}, {1, 0, 0, 0}, {c, d, 1, e}, {0, 0, 0, 1}};
        base.table = {{b}, {a}, {Mint(a + b)}, {1}};
        Matrix<Mint> res = transition.pow(N - 1) * base;
        cout << res[2][0] << '\n';
}

int main() {
        ios::sync_with_stdio(false);
        cin.tie(NULL);

        int t = 1;
        // cin >> t;
        while (t--) solve();

        return 0;
}

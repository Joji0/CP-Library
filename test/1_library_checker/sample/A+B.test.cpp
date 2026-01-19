#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "sample/A+B.hpp"
#include <bits/stdc++.h>
using namespace std;

void solve() {
        int64_t a, b;
        cin >> a >> b;
        cout << sum(a, b) << '\n';
}

int main() {
        ios::sync_with_stdio(false);
        cin.tie(NULL);

        int t = 1;
        // cin >> t;
        while (t--) solve();

        return 0;
}

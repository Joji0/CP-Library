#define PROBLEM "https://yukicoder.me/problems/no/81"
#include "bigdec/bigdec.hpp"
#include <bits/stdc++.h>
using namespace std;

void solve() {
        int N;
        cin >> N;
        BigDec<10> ans;
        for (int i = 0; i < N; i++) {
                BigDec<10> now;
                cin >> now;
                ans += now;
        }
        cout << ans << '\n';
}

int main() {
        ios::sync_with_stdio(false);
        cin.tie(NULL);

        int t = 1;
        // cin >> t;
        while (t--) solve();

        return 0;
}

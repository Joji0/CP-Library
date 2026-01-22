#define PROBLEM "https://yukicoder.me/problems/no/3198"
#include "ds/segtree/segtree.hpp"
#include <bits/stdc++.h>
using namespace std;

const int INF = 1 << 30, MAXN = 200005;

void solve() {
        int active = 0;
        int Q;
        cin >> Q;
        struct Monoid {
                using value_type = int;
                static int e() { return -INF; }
                static int op(const int &a, const int &b) { return max(a, b); }
        };
        SegTree<Monoid> seg(MAXN);
        while (Q--) {
                int t;
                cin >> t;
                if (t == 1) {
                        int x;
                        cin >> x;
                        seg.update(active, x);
                        active++;
                } else {
                        int k;
                        cin >> k;
                        cout << seg.query(active - k, active) << '\n';
                }
        }
}

int main() {
        ios::sync_with_stdio(false);
        cin.tie(NULL);

        int t = 1;
        // cin >> t;
        while (t--) solve();

        return 0;
}

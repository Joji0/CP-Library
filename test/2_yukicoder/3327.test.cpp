#define PROBLEM "https://yukicoder.me/problems/no/3327"
#include "ds/segtree/segtree.hpp"
#include <bits/stdc++.h>
using namespace std;

void solve() {
        int N, Q;
        cin >> N >> Q;
        vector<int> A(N);
        for (auto &x : A) cin >> x;
        struct Monoid {
                using value_type = int;
                static int e() { return 0; }
                static int op(const int &a, const int &b) { return max(a, b); }
        };
        SegTree<Monoid> seg(A);
        while (Q--) {
                int c, x;
                cin >> c >> x;
                if (c == 1) {
                        int pos = seg.find_first(0, N - 1, [&](const int &val) { return val > x; });
                        if (pos != -1) {
                                seg.update(pos, -1);
                                pos++;
                        }
                        cout << pos << '\n';
                } else {
                        int pos = seg.find_last(0, N - 1, [&](const int &val) { return val > x; });
                        if (pos != -1) {
                                seg.update(pos, -1);
                                pos++;
                        }
                        cout << pos << '\n';
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

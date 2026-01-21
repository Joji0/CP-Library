#include "ds/segtree/segtree.hpp"
#include <bits/stdc++.h>
using namespace std;

void solve() {
        int N, Q;
        cin >> N >> Q;
        vector<int> A(N);
        for (auto &x : A) cin >> x;
        struct Monoid {
                using value_type = pair<int, int>;
                static value_type e() { return {1 << 30, -1}; }
                static value_type op(const value_type &a, const value_type &b) { return min(a, b); }
        };
        vector<pair<int, int>> data(N);
        for (int i = 0; i < N; i++) {
                data[i] = {A[i], i + 1};
        }
        SegTree<Monoid> seg(data);
        while (Q--) {
                int t, l, r;
                cin >> t >> l >> r;
                l--, r--;
                if (t == 1) {
                        int pos1 = seg.get(l).first, pos2 = seg.get(r).first;
                        seg.update(l, {pos2, l + 1}), seg.update(r, {pos1, r + 1});
                } else {
                        auto mn = seg.query(l, r);
                        cout << mn.second << '\n';
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

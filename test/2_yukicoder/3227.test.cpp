#define PROBLEM "https://yukicoder.me/problems/no/3227"
#include "ds/matrix.hpp"
#include "ds/segtree/segtree.hpp"
#include "mod/dynamic_modint.hpp"
#include <bits/stdc++.h>
using namespace std;

void solve() {
        int K, N;
        cin >> K >> N;
        using dmint = DynamicModInt<0>;
        dmint::set_mod(K);
        using mat = Matrix<dmint>;
        vector<mat> A;
        for (int i = 0; i < N; i++) {
                mat now(2, 2);
                for (int x = 0; x < 2; x++) {
                        for (int y = 0; y < 2; y++) {
                                cin >> now[x][y];
                        }
                }
                A.push_back(now);
        }
        struct Monoid {
                using value_type = mat;
                static mat e() { return mat::identity(2); }
                static mat op(const mat &a, const mat &b) {
                        mat res = a * b;
                        return res;
                }
        };
        SegTree<Monoid> seg(A);
        int Q;
        cin >> Q;
        while (Q--) {
                int idx, l, r;
                cin >> idx >> l >> r;
                idx--, l--, r--;
                mat now(2, 2);
                for (int i = 0; i < 2; i++) {
                        for (int j = 0; j < 2; j++) {
                                cin >> now[i][j];
                        }
                }
                seg.update(idx, now);
                mat ans = seg.query(l, r);
                for (int i = 0; i < 2; i++) {
                        for (int j = 0; j < 2; j++) {
                                cout << ans[i][j] << " \n"[j == 1];
                        }
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

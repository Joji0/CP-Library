// 我思う、故に我在り
#define PROBLEM "https://judge.yosupo.jp/problem/jump_on_tree"
#include <bits/stdc++.h>
#include "graph/tree/LCA.hpp"

void solve() {
        int N, Q; std::cin >> N >> Q;
        Graph<int> G(N);
        for (int i = 0; i < N - 1; i++) {
                int a, b; std::cin >> a >> b;
                G.add_edge(a, b);
        }
        LCA<int> lca(G);
        while (Q--) {
                int s, t, i; std::cin >> s >> t >> i;
                int l = lca.query(s, t);
                int sl = lca.dist(s, l), lt = lca.dist(l, t);
                int tot = sl + lt;
                if (i > tot) {
                        std::cout << "-1\n";
                        continue;
                }
                if (i <= sl) {
                        std::cout << lca.lift(s, i) << '\n';
                } else {
                        int lft = tot - i;
                        std::cout << lca.lift(t, lft) << '\n';
                }
        }
}

int main() {
        std::ios::sync_with_stdio(false); std::cin.tie(nullptr);
        int tc = 1;
        // std::cin >> tc;
        while (tc--) solve();
        return 0;
}

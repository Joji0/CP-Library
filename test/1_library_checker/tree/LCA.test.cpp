// 我思う、故に我在り
#define PROBLEM "https://judge.yosupo.jp/problem/lca"
#include <bits/stdc++.h>
#include "graph/tree/LCA.hpp"

void solve() {
        int N, Q; std::cin >> N >> Q;
        Graph<> T(N);
        for (int i = 1; i < N; i++) {
                int p; std::cin >> p;
                T.add_edge(p, i);
        }
        LCA lca(T);
        while (Q--) {
                int u, v; std::cin >> u >> v;
                std::cout << lca.query(u, v) << '\n';
        }
}

int main() {
        std::ios::sync_with_stdio(false); std::cin.tie(nullptr);
        int tc = 1;
        // std::cin >> tc;
        while (tc--) solve();
        return 0;
}

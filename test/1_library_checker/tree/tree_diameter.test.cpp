// 我思う、故に我在り
#define PROBLEM "https://judge.yosupo.jp/problem/tree_diameter"
#include <bits/stdc++.h>
#include "graph/tree/TreeDiameter.hpp"

void solve() {
        int N; std::cin >> N;
        Graph<int64_t> G(N);
        for (int i = 0; i < N - 1; i++) {
                int a, b; int64_t c; std::cin >> a >> b >> c;
                G.add_edge(a, b, c);
        }
        TreeDiameter<int64_t> diam(G);
        std::cout << diam.diameter << " " << diam.path_nodes.size() << '\n';
        for (int v : diam.path_nodes) {
                std::cout << v << " ";
        }
        std::cout << '\n';
}

int main() {
        std::ios::sync_with_stdio(false); std::cin.tie(nullptr);
        int tc = 1;
        // std::cin >> tc;
        while (tc--) solve();
        return 0;
}

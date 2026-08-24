// 我思う、故に我在り
#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"
#include <bits/stdc++.h>
#include "math/convolution/Convolution.hpp"

void solve() {
        int N, M; std::cin >> N >> M;
        std::vector<modint998> A(N), B(M);
        for (auto &x : A) std::cin >> x;
        for (auto &x : B) std::cin >> x;
        std::vector<modint998> C = Convolution::convolution(A, B);
        for (auto &x : C) std::cout << x << " ";
        std::cout << '\n';
}

int main() {
        std::ios::sync_with_stdio(false); std::cin.tie(nullptr);
        int tc = 1;
        // std::cin >> tc;
        while (tc--) solve();
        return 0;
}

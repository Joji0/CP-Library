// 我思う、故に我在り
#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"
#include <bits/stdc++.h>
#include "ds/FastSet.hpp"

void solve() {
        int N, Q; std::cin >> N >> Q;
        FastSet fs(N);
        std::string s; std::cin >> s;
        fs.build(s);
        while (Q--) {
                char c; int k; std::cin >> c >> k;
                if (c == '0') {
                        fs.insert(k);
                } else if (c == '1') {
                        fs.erase(k);
                } else if (c == '2') {
                        if (fs.contains(k)) std::cout << "1\n";
                        else std::cout << "0\n";
                } else if (c == '3') {
                        std::cout << fs.next(k) << '\n';
                } else {
                        std::cout << fs.prev(k) << '\n';
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

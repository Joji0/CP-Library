// 我思う、故に我在り
#define PROBLEM "https://judge.yosupo.jp/problem/ordered_set"
#include <bits/stdc++.h>
#include "ds/OrderedSet.hpp"
#include "ds/OrderedMultiSet.hpp"

void solve() {
        int N, Q; std::cin >> N >> Q;
        OrderedSet<int> os;
        for (int i = 0; i < N; i++) {
                int x; std::cin >> x;
                os.insert(x);
        }
        while (Q--) {
                int t, x; std::cin >> t >> x;
                if (t == 0) {
                        os.insert(x);
                } else if (t == 1) {
                        os.erase(x);
                } else if (t == 2) {
                        auto opt = os.kth(x - 1);
                        if (opt) {
                                std::cout << *opt << '\n';
                        } else {
                                std::cout << "-1\n";
                        }
                } else if (t == 3) {
                        std::cout << os.count_less_equal(x) << '\n';
                } else if (t == 4) {
                        auto opt = os.max_less_equal(x);
                        if (opt) {
                                std::cout <<  *opt << '\n';
                        } else {
                                std::cout << "-1\n";
                        }
                } else {
                        auto opt = os.min_greater_equal(x);
                        if (opt) {
                                std::cout << *opt << '\n';
                        } else {
                                std::cout << "-1\n";
                        }
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

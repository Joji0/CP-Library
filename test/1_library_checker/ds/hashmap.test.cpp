#define PROBLEM "https://judge.yosupo.jp/problem/associative_array"
#include "ds/hashmap.hpp"
#include <bits/stdc++.h>
using namespace std;

void solve() {
        int Q;
        cin >> Q;
        // minimize resizing
        HashMap<int64_t, int64_t> hash_map(Q);
        while (Q--) {
                int t;
                cin >> t;
                if (!t) {
                        int64_t k, v;
                        cin >> k >> v;
                        hash_map[k] = v;
                } else {
                        int64_t k;
                        cin >> k;
                        cout << hash_map[k] << '\n';
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

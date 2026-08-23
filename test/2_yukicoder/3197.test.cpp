#define PROBLEM "https://yukicoder.me/problems/no/3197"
#include "ds/hashmap.hpp"
#include <bits/stdc++.h>
using namespace std;

void solve() {
        int N;
        cin >> N;
        HashMap<int, int> mp;
        for (int i = 0; i < N; i++) {
                int a;
                cin >> a;
                mp[a]++;
        }
        int Q;
        cin >> Q;
        while (Q--) {
                int x, k;
                cin >> x >> k;
                cout << (mp[x] >= k ? "Yes\n" : "No\n");
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

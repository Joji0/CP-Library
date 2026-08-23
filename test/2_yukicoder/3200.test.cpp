#define PROBLEM "https://yukicoder.me/problems/no/3200"
#include "ds/unionfind/unionfind.hpp"
#include <bits/stdc++.h>
using namespace std;

void solve() {
        int64_t N, M;
        cin >> N >> M;
        UnionFind uf(N);
        vector<array<int, 2>> Edges(M);
        multiset<array<int, 2>> st;
        for (int i = 0; i < M; i++) {
                cin >> Edges[i][0] >> Edges[i][1];
                Edges[i][0]--, Edges[i][1]--;
                if (Edges[i][0] > Edges[i][1]) {
                        swap(Edges[i][0], Edges[i][1]);
                }
                st.insert({Edges[i][0], Edges[i][1]});
        }
        int Q;
        cin >> Q;
        vector<int> query(Q);
        for (int i = 0; i < Q; i++) {
                int num_edge;
                cin >> num_edge;
                num_edge--;
                query[i] = num_edge;
                auto it = st.find(Edges[num_edge]);
                st.erase(it);
        }
        vector<int64_t> ans;
        for (auto &[u, v] : st) {
                uf.join(u, v);
        }
        reverse(query.begin(), query.end());
        int64_t connected = 0, tot = N * (N - 1) / 2;
        auto group = uf.groups();
        for (auto &root : group) {
                int64_t now = (int64_t)root.size();
                connected += now * (now - 1) / 2;
        }
        for (auto &num_edge : query) {
                ans.push_back(tot - connected);
                int par1 = uf.find(Edges[num_edge][0]), par2 = uf.find(Edges[num_edge][1]);
                int64_t size1 = uf.sz[par1], size2 = uf.sz[par2];
                if (par1 != par2) {
                        connected -= (size1 * (size1 - 1) / 2 + size2 * (size2 - 1) / 2);
                        int64_t new_size = size1 + size2;
                        connected += new_size * (new_size - 1) / 2;
                }
                uf.join(par1, par2);
        }
        reverse(ans.begin(), ans.end());
        for (auto &x : ans) {
                cout << x << '\n';
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

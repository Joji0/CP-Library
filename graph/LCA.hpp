#pragma once
#include <algorithm>
#include <vector>

struct LCA {
        int n, LOG;
        std::vector<std::vector<int>> G;
        std::vector<std::vector<int>> up;
        std::vector<int> depth, sub;
        LCA(int n = 0) { init(n); }
        void init(int n) {
                this->n = n;
                LOG = 1;
                while ((1 << LOG) <= n) LOG++;
                G.assign(n, {});
                up.assign(n, std::vector<int>(LOG, 0));
                depth.assign(n, 0);
                sub.assign(n, 1);
        }
        void add_edge(int u, int v) {
                G[u].push_back(v);
                G[v].push_back(u);
        }
        void dfs(int node, int par) {
                up[node][0] = par;
                for (int j = 1; j < LOG; j++) {
                        up[node][j] = up[up[node][j - 1]][j - 1];
                }
                for (int &nxt : G[node]) {
                        if (nxt == par) continue;
                        depth[nxt] = depth[node] + 1;
                        dfs(nxt, node);
                        sub[node] += sub[nxt];
                }
        }
        void build(int root = 0) {
                depth[root] = 0;
                dfs(root, root);
        }
        int lift(int u, int k) const {
                for (int j = LOG - 1; j >= 0; j--) {
                        if (k & (1 << j)) u = up[u][j];
                }
                return u;
        }
        int lca(int a, int b) const {
                if (depth[a] < depth[b]) std::swap(a, b);
                a = lift(a, depth[a] - depth[b]);
                if (a == b) return a;
                for (int j = LOG - 1; j >= 0; j--) {
                        if (up[a][j] != up[b][j]) {
                                a = up[a][j];
                                b = up[b][j];
                        }
                }
                return up[a][0];
        }
        int dist(int a, int b) const {
                int c = lca(a, b);
                return depth[a] + depth[b] - 2 * depth[c];
        }
};

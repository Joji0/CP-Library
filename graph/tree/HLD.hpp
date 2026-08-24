#pragma once
#include "graph/Graph.hpp"

template <typename T = int>
class HLD {
        int n;
        std::vector<int> parent, depth, sub, heavy, head, in, out;
        int timer;
        HLD(const Graph<T>& G, int root = 0) {
                n = G.size();
                parent.assign(n, -1);
                depth.assign(n, 0);
                sub.assign(n, 0);
                heavy.assign(n, -1);
                head.assign(n, 0);
                in.assign(n, 0);
                out.assign(n, 0);
                timer = 0;
                dfs_sz(G, root, -1, 0);
                dfs_hld(G, root, -1, root);
        }
        void dfs_sz(const Graph<T>& G, int v, int p, int d) {
                depth[v] = d;
                parent[v] = p;
                sub[v] = 1;
                for (int u : G[v]) {
                        if (u != p) {
                                dfs_sz(G, u, v, d + 1);
                                sub[v] += sub[u];
                                if (heavy[v] == -1 || sub[u] > sub[heavy[v]]) {
                                        heavy[v] = u;
                                }
                        }
                }
        }
        void dfs_hld(const Graph<T>& G, int v, int p, int h) {
                head[v] = h;
                in[v] = timer++;
                if (heavy[v] != -1) {
                        dfs_hld(G, heavy[v], v, h);
                }
                for (int u : G[v]) {
                        if (u != p && u != heavy[v]) {
                                dfs_hld(G, u, v, u);
                        }
                }
                out[v] = timer;
        }
public:
        int lca(int u, int v) const {
                while (head[u] != head[v]) {
                        if (depth[head[u]] < depth[head[v]]) std::swap(u, v);
                        u = parent[head[u]];
                }
                return depth[u] < depth[v] ? u : v;
        }
        template <class F>
        void path_query(int u, int v, bool is_edge, F f) const {
                while (head[u] != head[v]) {
                        if (depth[head[u]] < depth[head[v]]) std::swap(u, v);
                        f(in[head[u]], in[u]);
                        u = parent[head[u]];
                }
                if (depth[u] > depth[v]) std::swap(u, v);
                if (is_edge && u == v) return;
                f(in[u] + (is_edge ? 1 : 0), in[v]);
        }
        template <class F>
        void path_query_ordered(int u, int v, bool is_edge, F f) const {
                std::vector<std::tuple<int, int, bool>> left, right;
                while (head[u] != head[v]) {
                        if (depth[head[u]] >= depth[head[v]]) {
                                left.emplace_back(in[head[u]], in[u], true);
                                u = parent[head[u]];
                        } else {
                                right.emplace_back(in[head[v]], in[v], false);
                                v = parent[head[v]];
                        }
                }
                if (depth[u] >= depth[v]) {
                        int l = in[v] + (is_edge ? 1 : 0);
                        if (l <= in[u]) left.emplace_back(l, in[u], true);
                } else {
                        int l = in[u] + (is_edge ? 1 : 0);
                        if (l <= in[v]) right.emplace_back(l, in[v], true);
                }
                for (auto [l, r, rev] : left) {
                        f(l, r, rev);
                }
                for (auto it = right.rbegin(); it != right.rend(); ++it) {
                        auto [l, r, rev] = *it;
                        f(l, r, rev);
                }
        }
        template <class F>
        void subtree_query(int v, bool is_edge, F f) const {
                f(in[v] + (is_edge ? 1 : 0), out[v] - 1);
        }
};


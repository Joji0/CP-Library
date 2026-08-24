#pragma once
#include <algorithm>
#include <vector>
#include "graph/Graph.hpp"

template <typename T = int>
struct LCA {
	int n, LOG;
	std::vector<std::vector<int>> up;
	std::vector<int> depth;
        LCA(const Graph<T>& G, int root = 0) {
                n = G.size();
                LOG = 0;
                while ((1 << LOG) <= n) LOG++;
                depth.assign(n, 0);
                up.assign(n, std::vector<int>(LOG, 0));
                dfs(G, root, root, 0);
                for (int i = 1; i < LOG; i++) {
                        for (int v = 0; v < n; v++) {
                                up[v][i] = up[up[v][i - 1]][i - 1];
                        }
                }
        }
	void dfs(const Graph<T> &G, int v, int p, int d) {
                depth[v] = d;
		up[v][0] = p;
		for (int u : G[v]) {
                        if (u != p) {
                                dfs(G, u, v, d + 1);
                        }
		}
	}
	int lift(int v, int k) const {
		for (int j = LOG - 1; j >= 0; j--) {
			if (k & (1 << j)) v = up[v][j];
		}
		return v;
	}
	int query(int u, int v) const {
		if (depth[u] < depth[v]) std::swap(u, v);
		u = lift(u, depth[u] - depth[v]);
		if (u == v) return u;
		for (int j = LOG - 1; j >= 0; j--) {
			if (up[u][j] != up[v][j]) {
				u = up[u][j];
				v = up[v][j];
			}
		}
		return up[v][0];
	}
	int dist(int u, int v) const {
		return depth[u] + depth[v] - 2 * depth[query(u, v)];
	}
};

#pragma once
#include <vector>
#include <numeric>

struct KruskalReconstructionTree {
        int n, nodes;
        std::vector<int> par, lc, rc;
        std::vector<int> dsu_p;
        KruskalReconstructionTree(int n) : n(n), nodes(n) {
                par.assign(2 * n - 1, -1);
                lc.assign(2 * n - 1, -1);
                rc.assign(2 * n - 1, -1);
                dsu_p.resize(2 * n - 1);
                std::iota(dsu_p.begin(), dsu_p.end(), 0);
        }
        int find(int i) {
                return i == dsu_p[i] ? i : dsu_p[i] = find(dsu_p[i]);
        }
        int join(int u, int v) {
                u = find(u), v = find(v);
                if (u == v) return -1;
                int w = nodes++;
                par[u] = par[v] = w;
                lc[w] = u;
                rc[w] = v;
                dsu_p[u] = dsu_p[v] = w;
                return w;
        }
};

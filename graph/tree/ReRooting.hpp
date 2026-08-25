#pragma once
#include <vector>
#include <utility>
#include "graph/Graph.hpp"

template <class M, class T, class F>
class ReRooting {
private:
        using U = typename M::value_type;
        const F& f;
        int n;
        const Graph<T>& G;
        std::vector<U> init_data;
        std::vector<std::vector<U>> dp;
        std::vector<U> res;
        std::vector<int> par;
        void dfs1(int v, int p) {
                for (int i = 0; i < (int)G[v].size(); ++i) {
                        const auto& e = G[v][i];
                        if (e.to == p) {
                                par[v] = i;
                        } else {
                                dfs1(e.to, v);
                        }
                }
                for (int i = 0; i < (int)G[v].size(); ++i) {
                        const auto& e = G[v][i];
                        if (e.to == p) continue;
                        dp[v][par[v]] = M::op(
                                dp[v][par[v]],
                                f(dp[e.to][par[e.to]], Edge<T>(e.to, v, e.cost, e.id))
                        );
                }
                if (p != -1 && G[v].size() == 1) {
                        dp[v][par[v]] = init_data[v];
                }
        }
        void dfs2(int v, int p, int v_id) {
                std::vector<U> memo(G[v].size());
                for (int i = 0; i < (int)G[v].size(); ++i) {
                        const auto& e = G[v][i];
                        memo[i] = f(dp[e.to][e.to == p ? v_id : par[e.to]],
                                    Edge<T>(e.to, v, e.cost, e.id));
                }
                dp[v][G[v].size() - 1] = M::e();
                for (int i = (int)G[v].size() - 2; i >= 0; --i) {
                        dp[v][i] = M::op(memo[i + 1], dp[v][i + 1]);
                }
                U sml = M::e();
                for (int i = 0; i < (int)G[v].size(); ++i) {
                        dp[v][i] = M::op(sml, dp[v][i]);
                        sml = M::op(sml, std::move(memo[i]));
                }
                dp[v].back() = std::move(sml);
                if (G[v].size() == 1) {
                        dp[v][p == -1 ? 0 : par[v]] = init_data[v];
                }
                for (int i = 0; i < (int)G[v].size(); ++i) {
                        const auto& e = G[v][i];
                        if (e.to != p) {
                                dfs2(e.to, v, i);
                        }
                }
        }
        void init() {
                n = G.size();
                if (n == 1) {
                        res = init_data;
                        dp.assign(1, std::vector<U>{});
                        return;
                }
                dp.resize(n);
                for (int i = 0; i < n; ++i) {
                        dp[i].assign(G[i].size() + 1, M::e());
                }
                par.resize(n);
                par[0] = G[0].size();
                dfs1(0, -1);
                dfs2(0, -1, -1);
                res.resize(n);
                for (int i = 0; i < n; ++i) {
                        res[i] = std::move(dp[i].back());
                        dp[i].pop_back();
                }
        }
public:
        ReRooting(const Graph<T>& G, const F& f)
            : f(f), n(G.size()), G(G), init_data(std::vector<U>(G.size(), M::e())) {
                init();
        }
        ReRooting(const Graph<T>& G, const F& f, const std::vector<U>& ind)
            : f(f), n(G.size()), G(G), init_data(ind) {
                init();
        }
        ReRooting(const Graph<T>& G, const F& f, std::vector<U>&& ind)
            : f(f), n(G.size()), G(G), init_data(std::move(ind)) {
                init();
        }
        const std::vector<U>& get_res() const& { return res; }
        std::vector<U> get_res() && { return std::move(res); }
        const U& operator[](int v) const& { return res[v]; }
        U operator[](int v) && { return std::move(res[v]); }
        const std::vector<std::vector<U>>& get_dp() const& { return dp; }
        std::vector<std::vector<U>> get_dp() && { return std::move(dp); }
        const U& get_dp(int v, int p_idx) const& { return dp[v][p_idx]; }
        U get_dp(int v, int p_idx) && { return std::move(dp[v][p_idx]); }
};

template <class M, class T, class F>
ReRooting<M, T, F> make_rerooting(const Graph<T>& G, const F& f) {
        return ReRooting<M, T, F>(G, f);
}

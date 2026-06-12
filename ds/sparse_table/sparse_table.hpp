#pragma once
#include <algorithm>
#include <cassert>
#include <vector>

template <typename Monoid> struct SparseTable
{
        using T = typename Monoid::value_type;
        int n, LOG;
        std::vector<int> lg;
        std::vector<std::vector<T>> st;
        SparseTable() : n(0), LOG(0) {}
        SparseTable(const std::vector<T> &A) { init(A); }
        void init(const std::vector<T> &A)
        {
                n = (int)A.size();
                LOG = 1;
                while ((1 << LOG) <= n) LOG++;
                lg.assign(n + 1, 0);
                for (int i = 2; i <= n; i++) lg[i] = lg[i >> 1] + 1;
                st.assign(LOG, std::vector<T>(n, Monoid::e()));
                if (n == 0) return;
                st[0] = A;
                for (int i = 1; i < LOG; i++)
                {
                        for (int j = 0; j + (1 << i) <= n; j++)
                        {
                                st[i][j] = Monoid::op(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
                        }
                }
        }
        T query(int l, int r) const
        {
                assert(0 <= l && l <= r && r < n);
                int k = lg[r - l + 1];
                return Monoid::op(st[k][l], st[k][r - (1 << k) + 1]);
        }
};

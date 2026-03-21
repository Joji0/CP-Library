#pragma once

#include <algorithm>
#include <vector>
template <class T> struct SparseTable {
        int N = 0, LOG = 25;
        std::vector<std::vector<T>> st;
        T f(const T &a, const T &b) const { return std::min(a, b); }
        void init(const std::vector<T> &A) {
                N = (int)A.size();
                st.assign(LOG, vector<T>(N));
                for (int j = 0; j < N; j++) st[0][j] = A[j];
                for (int i = 1; i < LOG; i++) {
                        for (int j = 0; j + (1LL << i) - 1 < N; j++) {
                                st[i][j] = f(st[i - 1][j], st[i - 1][j + (1LL << (i - 1))]);
                        }
                }
        }
        T query(int L, int R) const {
                int len = R - L + 1;
                int lg = (int)log2(len);
                return f(st[lg][L], st[lg][R - (1 << lg) + 1]);
        }
};

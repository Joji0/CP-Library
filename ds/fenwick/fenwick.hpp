#pragma once
#include <cassert>
#include <cstdint>
#include <vector>

template <typename T = int64_t> struct Fenwick
{
        int n;
        std::vector<T> bit;
        Fenwick(int n = 0) { init(n); }
        void init(int n)
        {
                this->n = n;
                bit.assign(n + 1, T(0));
        }
        void add(int pos, T delta)
        {
                assert(0 <= pos && pos < n);
                for (int i = pos + 1; i <= n; i += i & -i) bit[i] += delta;
        }
        T sum_prefix(int r) const
        {
                assert(-1 <= r && r < n);
                T res = 0;
                for (int i = r + 1; i > 0; i -= i & -i) res += bit[i];
                return res;
        }
        T sum(int l, int r) const
        {
                if (l > r) return T(0);
                assert(0 <= l && r < n);
                return sum_prefix(r) - sum_prefix(l - 1);
        }
        int lower_bound(T target) const
        {
                if (target <= 0) return 0;
                int pos = 0;
                int pw = 1;
                while ((pw << 1) <= n) pw <<= 1;
                for (int k = pw; k > 0; k >>= 1)
                {
                        if (pos + k <= n && bit[pos + k] < target)
                        {
                                pos += k;
                                target -= bit[pos];
                        }
                }
                return pos < n ? pos : n;
        }
};

template <typename T = int64_t> struct RangeFenwick
{
        int n;
        Fenwick<T> bit1, bit2;
        RangeFenwick(int n = 0) { init(n); }
        void init(int n)
        {
                this->n = n;
                bit1.init(n + 1);
                bit2.init(n + 1);
        }
        void add_prefix(int pos, T a, T b)
        {
                bit1.add(pos, a);
                bit2.add(pos, b);
        }
        void range_add(int l, int r, T delta)
        {
                if (l > r) return;
                assert(0 <= l && r < n);
                add_prefix(l, delta, -delta * l);
                add_prefix(r + 1, -delta, delta * (r + 1));
        }
        T prefix_sum(int r) const
        {
                assert(-1 <= r && r < n);
                if (r < 0) return T(0);
                return bit1.sum_prefix(r) * (r + 1) + bit2.sum_prefix(r);
        }
        T range_sum(int l, int r) const
        {
                if (l > r) return T(0);
                assert(0 <= l && r < n);
                return prefix_sum(r) - prefix_sum(l - 1);
        }
        T point_query(int pos) const
        {
                assert(0 <= pos && pos < n);
                return range_sum(pos, pos);
        }
};

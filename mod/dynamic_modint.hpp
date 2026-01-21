#pragma once
#include <cassert>
#include <cstdint>
#include <iostream>
#include <utility>

template <int id> struct DynamicModInt {
        static inline uint32_t MOD = 998244353;
        uint32_t val;
        static void set_mod(uint32_t m) {
                assert(m > 0);
                MOD = m;
        }
        static uint32_t get_mod() { return MOD; }
        DynamicModInt() : val(0) {}
        DynamicModInt(const int64_t &x) : val(x >= 0 ? x % MOD : (MOD - (-x) % MOD) % MOD) {}
        uint32_t value() const { return val; }
        DynamicModInt &operator+=(const DynamicModInt &rhs) {
                val += rhs.val;
                if (val >= MOD) val -= MOD;
                return *this;
        }
        DynamicModInt &operator-=(const DynamicModInt &rhs) {
                if (val < rhs.val) val += MOD;
                val -= rhs.val;
                return *this;
        }
        DynamicModInt &operator*=(const DynamicModInt &rhs) {
                val = (uint64_t)val * rhs.val % MOD;
                return *this;
        }
        DynamicModInt &operator/=(const DynamicModInt &rhs) { return *this *= rhs.inverse(); }
        DynamicModInt operator+() const { return *this; }
        DynamicModInt operator-() const { return DynamicModInt(val == 0 ? 0 : MOD - val); }
        friend DynamicModInt operator+(DynamicModInt lhs, const DynamicModInt &rhs) { return lhs += rhs; }
        friend DynamicModInt operator-(DynamicModInt lhs, const DynamicModInt &rhs) { return lhs -= rhs; }
        friend DynamicModInt operator*(DynamicModInt lhs, const DynamicModInt &rhs) { return lhs *= rhs; }
        friend DynamicModInt operator/(DynamicModInt lhs, const DynamicModInt &rhs) { return lhs /= rhs; }
        friend bool operator==(const DynamicModInt &lhs, const DynamicModInt &rhs) { return lhs.val == rhs.val; }
        friend bool operator!=(const DynamicModInt &lhs, const DynamicModInt &rhs) { return lhs.val != rhs.val; }
        DynamicModInt pow(uint64_t n) const {
                DynamicModInt res = 1, a = *this;
                while (n > 0) {
                        if (n & 1) res *= a;
                        a *= a;
                        n >>= 1;
                }
                return res;
        }
        DynamicModInt inverse() const {
                int64_t a = val, b = MOD, u = 1, v = 0;
                while (b) {
                        int64_t t = a / b;
                        a -= t * b;
                        std::swap(a, b);
                        u -= t * v;
                        std::swap(u, v);
                }
                return DynamicModInt(u);
        }
        friend std::ostream &operator<<(std::ostream &os, const DynamicModInt &x) { return os << x.val; }
        friend std::istream &operator>>(std::istream &is, DynamicModInt &x) {
                int64_t v;
                is >> v;
                x = DynamicModInt(v);
                return is;
        }
};

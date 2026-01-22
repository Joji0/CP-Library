#pragma once
#include <cstdint>
#include <iostream>
#include <utility>

template <uint32_t MOD> struct ModInt {
        static_assert(MOD > 1, "MOD must be > 1");
        uint32_t val;
        static uint32_t mod() { return MOD; }
        constexpr ModInt() : val(0) {}
        constexpr ModInt(const int64_t &x) : val(x >= 0 ? x % MOD : (MOD - (-x) % MOD) % MOD) {}
        constexpr uint32_t value() const { return val; }
        constexpr ModInt &operator+=(const ModInt &rhs) {
                val += rhs.val;
                if (val >= MOD) val -= MOD;
                return *this;
        }
        constexpr ModInt &operator-=(const ModInt &rhs) {
                if (val < rhs.val) val += MOD;
                val -= rhs.val;
                return *this;
        }
        constexpr ModInt &operator*=(const ModInt &rhs) {
                val = (uint64_t)val * rhs.val % MOD;
                return *this;
        }
        constexpr ModInt &operator/=(const ModInt &rhs) { return *this *= rhs.inverse(); }
        constexpr ModInt operator+() const { return *this; }
        constexpr ModInt operator-() const { return ModInt(val == 0 ? 0 : MOD - val); }
        friend constexpr ModInt operator+(ModInt lhs, const ModInt &rhs) { return lhs += rhs; }
        friend constexpr ModInt operator-(ModInt lhs, const ModInt &rhs) { return lhs -= rhs; }
        friend constexpr ModInt operator*(ModInt lhs, const ModInt &rhs) { return lhs *= rhs; }
        friend constexpr ModInt operator/(ModInt lhs, const ModInt &rhs) { return lhs /= rhs; }
        friend constexpr bool operator==(const ModInt &lhs, const ModInt &rhs) { return lhs.val == rhs.val; }
        friend constexpr bool operator!=(const ModInt &lhs, const ModInt &rhs) { return lhs.val != rhs.val; }
        constexpr ModInt pow(uint64_t n) const {
                ModInt res = 1, a = *this;
                while (n > 0) {
                        if (n & 1) res *= a;
                        a *= a;
                        n >>= 1;
                }
                return res;
        }
        constexpr ModInt inverse() const {
                int64_t a = val, b = MOD, u = 1, v = 0;
                while (b) {
                        int64_t t = a / b;
                        a -= t * b;
                        std::swap(a, b);
                        u -= t * v;
                        std::swap(u, v);
                }
                return ModInt(u);
        }
        friend std::ostream &operator<<(std::ostream &os, const ModInt &x) { return os << x.val; }
        friend std::istream &operator>>(std::istream &is, ModInt &x) {
                int64_t v;
                is >> v;
                x = ModInt(v);
                return is;
        }
};

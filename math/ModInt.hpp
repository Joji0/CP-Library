#pragma once
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <limits>
#include <cassert>
#include "math/Barrett.hpp"

template <uint32_t Mod>
class StaticModInt {
        static_assert(Mod > 0, "Modulus must be positive");
        static_assert(Mod <= std::numeric_limits<uint32_t>::max() / 2,
                        "Modulus * 2 must be <= uint32_t::max()");
private:
        uint32_t val;
        static constexpr uint32_t inv1000000007[] = {
                0, 1, 500000004, 333333336, 250000002, 400000003,
                166666668, 142857144, 125000001, 111111112, 700000005
        };
        static constexpr uint32_t inv998244353[] = {
                0, 1, 499122177, 332748118, 748683265, 598946612,
                166374059, 855638017, 873463809, 443664157, 299473306
        };
        static constexpr uint32_t mod_inv(int64_t a) {
                int64_t b = Mod, x = 1, u = 0, t = 0, tmp = 0;
                while (b) {
                        t = a / b;
                        tmp = a - t * b;
                        a = b;
                        b = tmp;
                        tmp = x - t * u;
                        x = u;
                        u = tmp;
                }
                if (x < 0) x += Mod;
                return (uint32_t)x;
        }
public:
        constexpr StaticModInt() : val(0) {}
        template <class U, std::enable_if_t<std::is_integral_v<U> && std::is_signed_v<U>, int> = 0>
        constexpr StaticModInt(U v) {
                int64_t x = (int64_t)(v % (int64_t)Mod);
                if (x < 0) x += Mod;
                val = (uint32_t)x;
        }
        template <class U, std::enable_if_t<std::is_integral_v<U> && std::is_unsigned_v<U>, int> = 0>
        constexpr StaticModInt(U v) : val((uint32_t)(v % Mod)) {}
        constexpr uint32_t get() const { return val; }
        static constexpr uint32_t get_mod() { return Mod; }
        static constexpr StaticModInt raw(uint32_t v) {
                StaticModInt res;
                res.val = v;
                return res;
        }
        constexpr StaticModInt inv() const {
                if constexpr (Mod == 1000000007) {
                        if (val <= 10) return raw(inv1000000007[val]);
                } else if constexpr (Mod == 998244353) {
                        if (val <= 10) return raw(inv998244353[val]);
                }
                return raw(mod_inv(val));
        }
        constexpr StaticModInt& operator++() {
                if (++val == Mod) val = 0;
                return *this;
        }
        constexpr StaticModInt operator++(int) {
                StaticModInt res = *this;
                ++*this;
                return res;
        }
        constexpr StaticModInt& operator--() {
                if (val == 0) val = Mod;
                --val;
                return *this;
        }
        constexpr StaticModInt operator--(int) {
                StaticModInt res = *this;
                --*this;
                return res;
        }
        constexpr StaticModInt& operator+=(const StaticModInt& other) {
                val += other.val;
                if (val >= Mod) val -= Mod;
                return *this;
        }
        constexpr StaticModInt& operator-=(const StaticModInt& other) {
                if (val < other.val) val += Mod;
                val -= other.val;
                return *this;
        }
        constexpr StaticModInt& operator*=(const StaticModInt& other) {
                val = (uint32_t)((uint64_t)val * other.val % Mod);
                return *this;
        }
        constexpr StaticModInt& operator/=(const StaticModInt& other) {
                *this *= other.inv();
                return *this;
        }
        friend constexpr StaticModInt operator+(const StaticModInt& lhs, const StaticModInt& rhs) { return StaticModInt(lhs) += rhs; }
        friend constexpr StaticModInt operator-(const StaticModInt& lhs, const StaticModInt& rhs) { return StaticModInt(lhs) -= rhs; }
        friend constexpr StaticModInt operator*(const StaticModInt& lhs, const StaticModInt& rhs) { return StaticModInt(lhs) *= rhs; }
        friend constexpr StaticModInt operator/(const StaticModInt& lhs, const StaticModInt& rhs) { return StaticModInt(lhs) /= rhs; }
        constexpr StaticModInt operator+() const { return *this; }
        constexpr StaticModInt operator-() const { return StaticModInt() - *this; }
        friend constexpr bool operator==(const StaticModInt& lhs, const StaticModInt& rhs) { return lhs.val == rhs.val; }
        friend constexpr bool operator!=(const StaticModInt& lhs, const StaticModInt& rhs) { return lhs.val != rhs.val; }
        constexpr StaticModInt pow(int64_t a) const {
                StaticModInt v = *this, res = 1;
                while (a) {
                        if (a & 1) res *= v;
                        a >>= 1;
                        v *= v;
                }
                return res;
        }
        friend std::ostream& operator<<(std::ostream& os, const StaticModInt& m) { return os << m.val; }
        friend std::istream& operator>>(std::istream& is, StaticModInt& m) {
                int64_t v; is >> v;
                m = StaticModInt(v);
                return is;
        }
};

using modint107 = StaticModInt<1000000007>;
using modint998 = StaticModInt<998244353>;

template <int ID>
class DynamicModInt {
private:
        uint32_t val;
        static Barrett bt;
        static uint32_t mod_inv(int64_t a) {
                int64_t b = bt.mod, x = 1, u = 0, t = 0, tmp = 0;
                while (b) {
                        t = a / b;
                        tmp = a - t * b;
                        a = b;
                        b = tmp;
                        tmp = x - t * u;
                        x = u;
                        u = tmp;
                }
                if (x < 0) x += bt.mod;
                return (uint32_t)x;
        }
public:
        DynamicModInt() : val(0) {}
        template <class U, std::enable_if_t<std::is_integral_v<U> && std::is_signed_v<U>, int> = 0>
        DynamicModInt(U v) {
                int64_t x = (int64_t)(v % (int64_t)bt.mod);
                if (x < 0) x += bt.mod;
                val = (uint32_t)x;
        }
        template <class U, std::enable_if_t<std::is_integral_v<U> && std::is_unsigned_v<U>, int> = 0>
        DynamicModInt(U v) : val((uint32_t)(v % bt.mod)) {}
        uint32_t get() const { return val; }
        static uint32_t get_mod() { return bt.mod; }
        static void set_mod(uint32_t m) {
                assert(m > 0 && m <= std::numeric_limits<uint32_t>::max() / 2);
                bt = Barrett(m);
        }
        static DynamicModInt raw(uint32_t v) {
                DynamicModInt res;
                res.val = v;
                return res;
        }
        DynamicModInt inv() const { return raw(mod_inv(val)); }
        DynamicModInt& operator++() { if (++val == bt.mod) val = 0; return *this; }
        DynamicModInt operator++(int) { DynamicModInt res = *this; ++*this; return res; }
        DynamicModInt& operator--() { if (val == 0) val = bt.mod; --val; return *this; }
        DynamicModInt operator--(int) { DynamicModInt res = *this; --*this; return res; }
        DynamicModInt& operator+=(const DynamicModInt& other) { val += other.val; if (val >= bt.mod) val -= bt.mod; return *this; }
        DynamicModInt& operator-=(const DynamicModInt& other) { if (val < other.val) val += bt.mod; val -= other.val; return *this; }
        DynamicModInt& operator*=(const DynamicModInt& other) { val = bt.mul(val, other.val); return *this; }
        DynamicModInt& operator/=(const DynamicModInt& other) { *this *= other.inv(); return *this; }
        friend DynamicModInt operator+(const DynamicModInt& lhs, const DynamicModInt& rhs) { return DynamicModInt(lhs) += rhs; }
        friend DynamicModInt operator-(const DynamicModInt& lhs, const DynamicModInt& rhs) { return DynamicModInt(lhs) -= rhs; }
        friend DynamicModInt operator*(const DynamicModInt& lhs, const DynamicModInt& rhs) { return DynamicModInt(lhs) *= rhs; }
        friend DynamicModInt operator/(const DynamicModInt& lhs, const DynamicModInt& rhs) { return DynamicModInt(lhs) /= rhs; }
        DynamicModInt operator+() const { return *this; }
        DynamicModInt operator-() const { return DynamicModInt() - *this; }
        friend bool operator==(const DynamicModInt& lhs, const DynamicModInt& rhs) { return lhs.val == rhs.val; }
        friend bool operator!=(const DynamicModInt& lhs, const DynamicModInt& rhs) { return lhs.val != rhs.val; }
        DynamicModInt pow(int64_t a) const {
                DynamicModInt v = *this, res = 1;
                while (a) {
                        if (a & 1) res *= v;
                        a >>= 1;
                        v *= v;
                }
                return res;
        }
        friend std::ostream& operator<<(std::ostream& os, const DynamicModInt& m) { return os << m.val; }
        friend std::istream& operator>>(std::istream& is, DynamicModInt& m) {
                int64_t v; is >> v;
                m = DynamicModInt(v);
                return is;
        }
};

template <int ID>
Barrett DynamicModInt<ID>::bt = Barrett(998244353);

using dmint = DynamicModInt<0>;

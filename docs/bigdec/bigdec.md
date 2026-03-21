---
title: BigDec
documentation_of: bigdec/bigdec.hpp
---

# BigDec (Fixed-Point Decimal)

## Overview

A **fixed-point decimal** arithmetic type using `__int128_t` as the underlying storage. This allows exact decimal arithmetic with $P$ decimal places of precision, without floating-point rounding errors.

Useful in competitive programming for problems requiring:

- Exact decimal comparison (e.g., "is $a/b > c/d$?")
- Fixed-precision arithmetic where `double` is insufficient
- Reading decimal input without precision loss

The internal representation stores $\mathrm{val} = \mathrm{value} \times 10^P$ as a 128-bit integer, so the range is approximately $\pm 1.7 \times 10^{38} / 10^P$.

## Template Parameter

| Parameter | Description |
|-----------|-------------|
| `P` | Number of decimal places (precision). For example, `BigDec<9>` stores 9 decimal digits after the point. |

## Constructors

### `BigDec<P>()`

Creates a BigDec with value $0$.

### `BigDec<P>(int64_t n)`

Creates a BigDec with integer value $n$ (i.e., $n.000\ldots$).

**Complexity**: $O(P)$

### `BigDec<P>(std::string s)`

Creates a BigDec by parsing a decimal string like `"3.14159"`. The string may include a negative sign, integer part, and fractional part. Digits beyond $P$ decimal places are truncated.

**Complexity**: $O(|s|)$

## Operators

Full arithmetic and comparison operators:

| Operator | Description | Complexity |
|----------|-------------|------------|
| `a + b`, `a - b` | Addition / subtraction | $O(1)$ |
| `a * b` | Multiplication (result truncated to $P$ decimals) | $O(1)$ |
| `a / b` | Division (result truncated to $P$ decimals) | $O(1)$ |
| `+a`, `-a` | Unary plus / negation | $O(1)$ |
| `a < b`, `a > b`, `a <= b`, `a >= b` | Comparison | $O(1)$ |
| `a == b`, `a != b` | Equality | $O(1)$ |
| `cin >> a` | Read decimal string from input | $O(|s|)$ |
| `cout << a` | Output with exactly $P$ decimal digits | $O(P)$ |

All compound assignment versions (`+=`, `-=`, `*=`, `/=`) are also available.

## Members

| Member | Description |
|--------|-------------|
| `val` | The internal `__int128_t` representation: actual value $\times 10^P$ |

## Precision Notes

- Multiplication: $(a \times b)$ internally computes a 256-bit product $a.\mathrm{val} \times b.\mathrm{val}$ and divides by $10^P$, which may lose the lowest bits.
- Division: similarly, the result is $a.\mathrm{val} \times 10^P / b.\mathrm{val}$.
- For very large values (close to `__int128_t` limits), multiplication and division may overflow.
- Safe range (approximate): values up to $\sim 10^{38-P}$.

## Usage Example

```cpp
#include "bigdec/bigdec.hpp"

int main() {
    using Dec = BigDec<9>; // 9 decimal places

    Dec a("3.141592653");
    Dec b("2.718281828");

    Dec c = a + b;  // 5.859874481
    Dec d = a * b;  // 8.539734222 (truncated to 9 decimals)
    Dec e = a / b;  // 1.155727349

    if (a > b) { /* true */ }

    std::cin >> a;   // reads a decimal number
    std::cout << a;  // prints with exactly 9 decimal places
}
```

## Source Code

```cpp
#pragma once
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>

template <int P> struct BigDec {
        using i128 = __int128_t;
        i128 val;
        static i128 get_scale() {
                i128 x = 1;
                for (int i = 0; i < P; i++) x *= 10;
                return x;
        }
        BigDec() : val(0) {}
        BigDec(int64_t n) : val((i128)n * get_scale()) {}
        BigDec(std::string s) { read_string(s); }
        void read_string(std::string s) {
                val = 0;
                if (s.empty()) return;
                bool negative = (s[0] == '-');
                if (negative) s = s.substr(1);
                size_t dot = s.find('.');
                std::string s_int = (dot == std::string::npos) ? s : s.substr(0, dot);
                std::string s_frac = (dot == std::string::npos) ? "" : s.substr(dot + 1);
                i128 int_part = 0;
                if (!s_int.empty()) {
                        for (char c : s_int) {
                                int_part = int_part * 10 + (c - '0');
                        }
                }
                i128 frac_part = 0;
                if (s_frac.length() > P) s_frac = s_frac.substr(0, P);
                while (s_frac.length() < P) s_frac += "0";
                for (char c : s_frac) frac_part = frac_part * 10 + (c - '0');
                val = int_part * get_scale() + frac_part;
                if (negative) val = -val;
        }
        BigDec operator+(const BigDec &other) const {
                BigDec ret;
                ret.val = val + other.val;
                return ret;
        }
        BigDec &operator+=(const BigDec &other) {
                val += other.val;
                return *this;
        }
        BigDec operator-(const BigDec &other) const {
                BigDec ret;
                ret.val = val - other.val;
                return ret;
        }
        BigDec &operator-=(const BigDec &other) {
                val -= other.val;
                return *this;
        }
        BigDec operator+() const {
                BigDec ret;
                ret.val = val;
                return ret;
        }
        BigDec operator-() const {
                BigDec ret;
                ret.val = -val;
                return ret;
        }
        BigDec operator*(const BigDec &other) const {
                BigDec ret;
                ret.val = (val * other.val) / get_scale();
                return ret;
        }
        BigDec &operator*=(const BigDec &other) {
                val = (val * other.val) / get_scale();
                return *this;
        }
        BigDec operator/(const BigDec &other) const {
                BigDec ret;
                ret.val = (val * get_scale()) / other.val;
                return ret;
        }
        BigDec &operator/=(const BigDec &other) {
                val = (val * get_scale()) / other.val;
                return *this;
        }
        bool operator<(const BigDec &other) const { return val < other.val; }
        bool operator>(const BigDec &other) const { return val > other.val; }
        bool operator<=(const BigDec &other) const { return val <= other.val; }
        bool operator>=(const BigDec &other) const { return val >= other.val; }
        bool operator==(const BigDec &other) const { return val == other.val; }
        bool operator!=(const BigDec &other) const { return val != other.val; }
        friend std::istream &operator>>(std::istream &is, BigDec &bd) {
                std::string s;
                is >> s;
                bd.read_string(s);
                return is;
        }
        friend std::ostream &operator<<(std::ostream &os, const BigDec &bd) {
                i128 temp = bd.val;
                if (temp < 0) {
                        os << "-";
                        temp = -temp;
                }
                i128 scale = get_scale();
                i128 int_part = temp / scale;
                i128 frac_part = temp % scale;
                std::string s_int = "";
                if (int_part == 0)
                        s_int = "0";
                else {
                        i128 t = int_part;
                        while (t > 0) {
                                s_int += (char)('0' + (t % 10));
                                t /= 10;
                        }
                        std::reverse(s_int.begin(), s_int.end());
                }
                os << s_int;
                os << ".";
                std::string s_frac = "";
                i128 t_frac = frac_part;
                for (int i = 0; i < P; ++i) {
                        s_frac += (char)('0' + (t_frac % 10));
                        t_frac /= 10;
                }
                std::reverse(s_frac.begin(), s_frac.end());
                os << s_frac;
                return os;
        }
};

```

---
title: DynamicModInt
documentation_of: mod/dynamic_modint.hpp
---

# DynamicModInt

## Overview

A **runtime** modular integer type where the modulus $M$ can be set (and changed) at runtime. Uses [Barrett Reduction](./barrett.md) for efficient modular multiplication without division instructions.

The interface is identical to [ModInt](./modint.md), but the modulus is stored as a static member rather than a template parameter. This allows:

- Setting the modulus at runtime (e.g., reading it from input)
- Multiple independent modular contexts via different `id` template parameters

## Template Parameter

| Parameter | Description |
|-----------|-------------|
| `id` | An `int` to distinguish different modular contexts. Two `DynamicModInt` types with different `id`s maintain separate moduli. |

### Common Instantiation

```cpp
using Mint = DynamicModInt<0>;
Mint::set_mod(998244353);
```

**Multiple moduli:**
```cpp
using Mint1 = DynamicModInt<0>;
using Mint2 = DynamicModInt<1>;
Mint1::set_mod(998244353);
Mint2::set_mod(1000000007);
```

## Dependency

- [mod/barrett.hpp](./barrett.md)

## Static Methods

### `void set_mod(uint32_t m)`

Sets the modulus to $m$.

**Constraints**: $m > 0$

**Complexity**: $O(1)$

### `uint32_t mod()`

Returns the current modulus.

**Complexity**: $O(1)$

## Constructor

### `DynamicModInt()`

Creates a DynamicModInt with value $0$.

### `DynamicModInt(int64_t x)`

Creates a DynamicModInt with value $x \bmod M$. Handles negative values correctly.

**Constraints**: `set_mod()` must have been called.

**Complexity**: $O(1)$

## Operators

Identical to [ModInt](./modint.md):

| Operator | Complexity |
|----------|------------|
| `+`, `-`, `*`, `/` | $O(1)$ (division: $O(\log M)$) |
| `+=`, `-=`, `*=`, `/=` | Same as above |
| `+a`, `-a` | $O(1)$ |
| `==`, `!=` | $O(1)$ |
| `cin >>`, `cout <<` | $O(1)$ |

## Methods

### `uint32_t value()`

Returns the stored value in $[0, M)$.

### `DynamicModInt pow(uint64_t n)`

Returns $\mathrm{this}^n \bmod M$ using binary exponentiation.

**Complexity**: $O(\log n)$

### `DynamicModInt inverse()`

Returns $\mathrm{this}^{-1} \bmod M$ using the extended Euclidean algorithm.

**Complexity**: $O(\log M)$

## Usage Example

```cpp
#include "mod/dynamic_modint.hpp"

using Mint = DynamicModInt<0>;

int main() {
    int mod;
    std::cin >> mod;
    Mint::set_mod(mod);

    Mint a = 3, b = 5;
    Mint c = a * b;
    Mint d = a.pow(1000000);
    Mint e = b.inverse();
}
```

## Source Code

```cpp
#pragma once
#include "mod/barrett.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <utility>

template <int id> struct DynamicModInt {
        static inline Barrett MOD = Barrett(998244353);
        uint32_t val;
        static void set_mod(uint32_t m) {
                assert(m > 0);
                MOD = Barrett(m);
        }
        static uint32_t mod() { return MOD.umod(); }
        DynamicModInt() : val(0) {}
        DynamicModInt(const int64_t &x)
            : val(x >= 0 ? x % MOD.umod() : (MOD.umod() - (-x) % MOD.umod()) % MOD.umod()) {}
        uint32_t value() const { return val; }
        DynamicModInt &operator+=(const DynamicModInt &rhs) {
                val += rhs.val;
                if (val >= MOD.umod()) val -= MOD.umod();
                return *this;
        }
        DynamicModInt &operator-=(const DynamicModInt &rhs) {
                if (val < rhs.val) val += MOD.umod();
                val -= rhs.val;
                return *this;
        }
        DynamicModInt &operator*=(const DynamicModInt &rhs) {
                val = MOD.mul(val, rhs.val);
                return *this;
        }
        DynamicModInt &operator/=(const DynamicModInt &rhs) { return *this *= rhs.inverse(); }
        DynamicModInt operator+() const { return *this; }
        DynamicModInt operator-() const { return DynamicModInt(val == 0 ? 0 : MOD.umod() - val); }
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
                int64_t a = val, b = MOD.umod(), u = 1, v = 0;
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
```

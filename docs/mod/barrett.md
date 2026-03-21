---
title: Barrett Reduction
documentation_of: mod/barrett.hpp
---

# Barrett Reduction

## Overview

Barrett Reduction is a technique for performing modular arithmetic **without division instructions**, replacing them with multiplications and bit shifts. This is significantly faster when the modulus is not a compile-time constant.

The key idea: given a modulus $m$, precompute $\mathrm{im} = \left\lceil \frac{2^{64}}{m} \right\rceil$. Then for any $z$, the quotient $\lfloor z / m \rfloor$ can be approximated as:

$$x \approx \frac{z \cdot \mathrm{im}}{2^{64}}$$

The division by $2^{64}$ is a free bit-shift (taking the upper 64 bits of a 128-bit product). This eliminates the expensive `div` instruction, replacing it with a single `mul` + shift.

The implementation is based on [Maspy's library](https://maspypy.github.io/library/mod/barrett.hpp) and the [AtCoder Library internal math](https://github.com/atcoder/ac-library/blob/master/atcoder/internal_math.hpp).

## Constructor

### `Barrett(uint32_t m = 1)`

Precomputes the Barrett constant $\mathrm{im} = \left\lceil 2^{64} / m \right\rceil$ for modulus $m$.

**Constraints**: $m \ge 1$

**Complexity**: $O(1)$

## Methods

### `uint32_t umod()`

Returns the modulus $m$.

**Complexity**: $O(1)$

### `uint32_t modulo(uint64_t z)`

Returns $z \bmod m$.

**Constraints**: $0 \le z < 2^{64}$

**Complexity**: $O(1)$

### `uint64_t floor(uint64_t z)`

Returns $\lfloor z / m \rfloor$.

**Constraints**: $0 \le z < 2^{64}$

**Complexity**: $O(1)$

### `std::pair<uint64_t, uint32_t> divmod(uint64_t z)`

Returns $(\lfloor z / m \rfloor, \; z \bmod m)$ as a pair.

**Constraints**: $0 \le z < 2^{64}$

**Complexity**: $O(1)$

### `uint32_t mul(uint32_t a, uint32_t b)`

Returns $(a \times b) \bmod m$.

**Constraints**: $0 \le a, b < m$

**Complexity**: $O(1)$

## Usage Example

```cpp
#include "mod/barrett.hpp"

int main() {
    Barrett bt(998244353);

    bt.modulo(1000000000000LL);  // 1000000000000 % 998244353
    bt.floor(1000000000000LL);   // 1000000000000 / 998244353
    bt.mul(500000000, 600000000); // (5e8 * 6e8) % 998244353

    auto [q, r] = bt.divmod(1000000000000LL);
    // q = quotient, r = remainder
}
```

## When to Use

- **DynamicModInt**: This struct is used internally by [DynamicModInt](./dynamic_modint.md) to avoid division in modular multiplication.
- **Manual modular arithmetic**: When you need fast mod operations with a runtime modulus but don't want the full `DynamicModInt` wrapper.
- **Performance-critical inner loops**: Replacing `%` with Barrett in tight loops can give 2-3x speedup.

## Source Code

```cpp
#pragma once
#include <cstdint>
#include <utility>

// reference: https://maspypy.github.io/library/mod/barrett.hpp
struct Barrett {
        uint32_t mod;
        uint64_t inv_mod;
        explicit Barrett(uint32_t m = 1) : mod(m), inv_mod((uint64_t)(-1) / m + 1) {}
        uint32_t umod() const { return mod; }
        uint32_t modulo(uint64_t z) const {
                if (mod == 1) return 0;
                uint64_t x = (uint64_t)(((__uint128_t)(z)*inv_mod) >> 64);
                uint64_t y = x * mod;
                return (uint32_t)(z - y + (z < y ? mod : 0));
        }
        uint64_t floor(uint64_t z) const {
                if (mod == 1) return z;
                uint64_t x = (uint64_t)(((__uint128_t)(z)*inv_mod) >> 64);
                uint64_t y = x * mod;
                return (z < y ? x - 1 : x);
        }
        std::pair<uint64_t, uint32_t> divmod(uint64_t z) const {
                if (mod == 1) return {z, 0};
                uint64_t x = (uint64_t)(((__uint128_t)(z)*inv_mod) >> 64);
                uint64_t y = x * mod;
                if (z < y) return {x - 1, z - y + mod};
                return {x, z - y};
        }
        uint32_t mul(uint32_t a, uint32_t b) const { return modulo((uint64_t)(a)*b); }
};

```

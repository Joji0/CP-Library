---
title: DynamicModInt
documentation_of: mod/dynamic_modint.hpp
---

Runtime mod arithmetic using Barrett reduction.

### Usage

- `DynamicModInt<id>::set_mod(m)`: set mod to $m$
- `DynamicModInt<id>(x)`: initialize with $x \bmod m$
- `+`, `-`, `*`, `/`: arithmetic operations
- `pow(k)`: $a^k$
- `inverse()`: $a^{-1}$

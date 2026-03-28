---
title: Barrett Reduction
documentation_of: //mod/barrett.hpp
---

Fast modular arithmetic via Barrett reduction. Used internally by DynamicModInt.

### Usage

- `Barrett(mod)`: initialize with mod $m$
- `modulo(a)`: $a \bmod m$
- `mul(a, b)`: $a \cdot b \bmod m$

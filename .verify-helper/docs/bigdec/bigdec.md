---
title: BigDec
documentation_of: bigdec/bigdec.hpp
---

Fixed-point big decimal arithmetic using `__int128_t` internally.

### Usage

- `BigDec<D>(string s)`: construct from string ($D$ = decimal places)
- `BigDec<D>(int64_t x)`: construct from integer
- `+`, `-`, `*`: arithmetic operations
- `<`, `<=`, `==`, `!=`, `>`, `>=`: comparisons
- `cin >> a`, `cout << a`: I/O

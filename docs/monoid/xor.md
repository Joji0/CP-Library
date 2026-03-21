---
title: XorMonoid
documentation_of: monoid/xor.hpp
---

# XorMonoid

## Overview

Monoid for **range XOR** queries on a segment tree.

| Property | Value |
|----------|-------|
| Set $S$ | `int64_t` |
| Operation $\cdot$ | $a \oplus b$ |
| Identity $e$ | $0$ |

Note: XOR is its own inverse, so this monoid is also a group.

## Usage

```cpp
#include "monoid/xor.hpp"
#include "ds/segtree/segtree.hpp"

SegTree<XorMonoid> seg(n); // range xor, point update
```

## Source Code

```cpp
#pragma once

struct XorMonoid {
	using value_type = int64_t;
	static value_type e() { return 0; }
	static value_type op(value_type a, value_type b) { return a ^ b; }
};

```

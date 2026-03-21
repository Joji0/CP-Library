---
title: AddMonoid
documentation_of: monoid/add.hpp
---

# AddMonoid

## Overview

Monoid for **range sum** queries on a segment tree.

| Property | Value |
|----------|-------|
| Set $S$ | `int64_t` |
| Operation $\cdot$ | $a + b$ |
| Identity $e$ | $0$ |

## Usage

```cpp
#include "monoid/add.hpp"
#include "ds/segtree/segtree.hpp"

SegTree<AddMonoid> seg(n); // range sum, point update
```

## Source Code

```cpp
#pragma once

struct AddMonoid {
	using value_type = int64_t;
	static value_type e() { return 0; }
	static value_type op(value_type a, value_type b) { return a + b; }
};

```

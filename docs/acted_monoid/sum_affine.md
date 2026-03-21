---
title: SumAffine
documentation_of: acted_monoid/sum_affine.hpp
---

# SumAffine — Range Affine, Range Sum

## Overview

ActedMonoid for lazy segment tree supporting **range affine transformation** $x \mapsto ax + b$ and **range sum** queries.

This is one of the most powerful and frequently used acted monoids in competitive programming, appearing in problems that combine linear transformations with sum queries.

| Component | Type | Description |
|-----------|------|-------------|
| $S$ | `struct {long long val; int cnt;}` | Node stores sum and segment size |
| $F$ | `struct {long long a, b;}` | Affine: $x \mapsto ax + b$ |
| $\mathrm{op}(s_1, s_2)$ | `{s1.val+s2.val, s1.cnt+s2.cnt}` | Merge two nodes |
| $e$ | `{0, 0}` | Identity |
| $\mathrm{mapping}(f, x)$ | `{f.a*x.val + f.b*x.cnt, x.cnt}` | Apply $f$ to segment |
| $\mathrm{composition}(f, g)$ | `{f.a*g.a, f.a*g.b + f.b}` | $f \circ g$: apply $g$ then $f$ |
| $\mathrm{id}$ | `{1, 0}` | Identity transformation $x \mapsto x$ |

### Composition Derivation

If $g(x) = g_a x + g_b$ and $f(x) = f_a x + f_b$, then:

$$f(g(x)) = f_a(g_a x + g_b) + f_b = (f_a \cdot g_a)x + (f_a \cdot g_b + f_b)$$

## Important

- Leaves must have `cnt = 1`.
- **Assign** is a special case: $f = \{0, v\}$ assigns value $v$ to all elements.
- **Add** is a special case: $f = \{1, v\}$ adds $v$ to all elements.
- **Multiply** is a special case: $f = \{v, 0\}$ multiplies all elements by $v$.

## Usage

```cpp
#include "acted_monoid/sum_affine.hpp"
#include "ds/segtree/lazy_segtree.hpp"

std::vector<SumAffine::S> a;
for (int x : {3, 1, 4}) a.push_back({x, 1});
LazySegTree<SumAffine> seg(a);

seg.query(0, 2).val;                  // 8
seg.update(0, 2, SumAffine::F{2, 1}); // 2x+1 → {7, 3, 9}
seg.query(0, 2).val;                  // 19
seg.update(0, 2, SumAffine::F{0, 5}); // assign 5 → {5, 5, 5}
seg.query(0, 2).val;                  // 15
```

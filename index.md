---
layout: default
title: Home
---

<div class="hero">
  <h1>CP-Library</h1>
  <p class="hero-subtitle">
    A competitive programming library in C++. Header-only, well-tested, and documented in ACL style.
  </p>
  <div class="hero-stats">
    <div class="hero-stat">
      <div class="stat-value">32</div>
      <div class="stat-label">Templates</div>
    </div>
    <div class="hero-stat">
      <div class="stat-value">6</div>
      <div class="stat-label">Categories</div>
    </div>
    <div class="hero-stat">
      <div class="stat-value">O(log n)</div>
      <div class="stat-label">Per Query</div>
    </div>
  </div>
</div>

<div class="category-grid">

<a class="category-card" href="{{ '/docs/ds/segtree/segtree' | relative_url }}">
  <div class="card-icon">&#127795;</div>
  <h3>Segment Trees</h3>
  <p>SegTree, LazySegTree, PersistentSegTree &mdash; point/range update, range query, binary search on tree</p>
  <div class="card-count">3 templates</div>
</a>

<a class="category-card" href="{{ '/docs/ds/unionfind/unionfind' | relative_url }}">
  <div class="card-icon">&#128279;</div>
  <h3>Union-Find</h3>
  <p>DSU with path compression + union by size, rollback variant for offline algorithms</p>
  <div class="card-count">2 templates</div>
</a>

<a class="category-card" href="{{ '/docs/ds/treap/implicit_treap' | relative_url }}">
  <div class="card-icon">&#127922;</div>
  <h3>Implicit Treap</h3>
  <p>Sequence with insert/erase, range update/query/reverse &mdash; the Swiss army knife of data structures</p>
  <div class="card-count">1 template</div>
</a>

<a class="category-card" href="{{ '/docs/ds/ordered_set' | relative_url }}">
  <div class="card-icon">&#128202;</div>
  <h3>Order Statistics</h3>
  <p>OrderedSet, OrderedMultiSet &mdash; k-th element, order of key, range counting in O(log n)</p>
  <div class="card-count">2 templates</div>
</a>

<a class="category-card" href="{{ '/docs/ds/hashmap' | relative_url }}">
  <div class="card-icon">&#9889;</div>
  <h3>Other DS</h3>
  <p>HashMap (anti-hash), SparseTable (O(1) RMQ), Matrix (exponentiation)</p>
  <div class="card-count">3 templates</div>
</a>

<a class="category-card" href="{{ '/docs/monoid/add' | relative_url }}">
  <div class="card-icon">&#129518;</div>
  <h3>Monoid</h3>
  <p>Plug-and-play monoids: Add, Max, Min, Xor, Gcd, Mul &mdash; drop into SegTree directly</p>
  <div class="card-count">6 templates</div>
</a>

<a class="category-card" href="{{ '/docs/acted_monoid/sum_add' | relative_url }}">
  <div class="card-icon">&#127919;</div>
  <h3>ActedMonoid</h3>
  <p>Range Add/Assign/Affine &times; Sum/Max/Min &mdash; all 9 combinations for LazySegTree</p>
  <div class="card-count">9 templates</div>
</a>

<a class="category-card" href="{{ '/docs/mod/modint' | relative_url }}">
  <div class="card-icon">&#128290;</div>
  <h3>Modular Arithmetic</h3>
  <p>ModInt, DynamicModInt, Barrett reduction, Combinatorics (C/P/H, Catalan, derangements)</p>
  <div class="card-count">4 templates</div>
</a>

<a class="category-card" href="{{ '/docs/nt/sieve' | relative_url }}">
  <div class="card-icon">&#128269;</div>
  <h3>Number Theory</h3>
  <p>Linear sieve: primes, SPF, Euler's totient, M&ouml;bius, factorization, divisors</p>
  <div class="card-count">1 template</div>
</a>

<a class="category-card" href="{{ '/docs/bigdec/bigdec' | relative_url }}">
  <div class="card-icon">&#128208;</div>
  <h3>BigDec</h3>
  <p>Fixed-point decimal with __int128_t &mdash; exact arithmetic without floating-point errors</p>
  <div class="card-count">1 template</div>
</a>

</div>

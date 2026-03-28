---
title: HashMap
documentation_of: ds/hashmap.hpp
---

Open addressing hash map with splitmix64. Anti-hack.

### Usage

- `HashMap<K, V>(int n)`: initialize with capacity $n$
- `insert(key, val)`: insert entry
- `operator[](key)`: access by key
- `get(key)`: return value if key exists
- `sz()`: number of elements
- `build(keys, vals)`: batch construction

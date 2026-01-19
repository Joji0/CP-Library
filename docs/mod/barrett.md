# Barrett Reduction

Barrett Reduction is used to do modular arithmetic for some non-constant modulo fast. This requires some mathematical observation which can be seen in [Wikipedia](https://en.wikipedia.org/wiki/Barrett_reduction). The implementation used here is due to [Maspy](https://maspypy.github.io/library/mod/barrett.hpp) and [ACL](https://github.com/atcoder/ac-library/blob/master/atcoder/internal_math.hpp). 

Given a number $z$ we want to answer $z\mod m$ fast. The naive implementation is to use $z - m(z / m)$ where $z/m$ is the floor division in some programming language. Doing this repetitively will cost us lots of time due to the inefficiency of division compared to that of the multiplication. The idea of Barrett Reduction is to instead find an approximation of $z / m$. Indeed we can choose some very huge number that will be beneficial for the computer, say $2 ^ {64}$. We will then compute $\mathrm{inv\_mod} = \frac{2^{64}}{m}$ and represent $z$ as

$$x \approx z\cdot\frac{\text{inv\\_mod}}{2^{64}}$$

where $x$ is the quotient $z / m$. Notice that $\mathrm{inv\\_mod}$ can be calculated only once (so we can think of it as precomputation) and use this number everytime we need it without doing inefficient division again. The advantage of this reformulation is that dividing $\mathrm{inv\_mod}$ with $2^{64}$ is equivalent with shifting 64 bit to the right, which can be done really fast by the CPU. 

Below is the implementation,
```cpp
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
        uint32_t mul(uint32_t a, uint32_t b) { return modulo((uint64_t)(a)*b); }
};
```

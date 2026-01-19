---
title: Barrett Reduction
documentation_of: mod/barrett.hpp
---

# Barrett Reduction

Barrett Reduction is used to do modular arithmetic for some non-constant modulo fast. This requires some mathematical observation which can be seen in [Wikipedia](https://en.wikipedia.org/wiki/Barrett_reduction). The implementation used here is due to [Maspy](https://maspypy.github.io/library/mod/barrett.hpp) and [ACL](https://github.com/atcoder/ac-library/blob/master/atcoder/internal_math.hpp). 

Given a number $z$ we want to answer $z\mod m$ fast. The naive implementation is to use $z - m(z / m)$ where $z/m$ is the floor division in some programming language. Doing this repetitively will cost us lots of time due to the inefficiency of division compared to that of the multiplication. The idea of Barrett Reduction is to instead find an approximation of $z / m$. Indeed we can choose some very huge number that will be beneficial for the computer, say $2 ^ {64}$. We will then compute $\mathrm{im} = \frac{2^{64}}{m}$ and represent $z$ as

$$x \approx z\cdot\frac{\text{im}}{2^{64}}$$

where $x$ is the quotient $z / m$. Notice that $\mathrm{im}$ can be calculated only once (so we can think of it as precomputation) and use this number everytime we need it without doing inefficient division again. The advantage of this reformulation is that dividing $\mathrm{im}$ with $2^{64}$ is equivalent with shifting 64 bit to the right, which can be done really fast by the CPU. 

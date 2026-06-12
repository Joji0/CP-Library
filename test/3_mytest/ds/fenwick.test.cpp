#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"
#include <bits/stdc++.h>
using namespace std;
#include "ds/fenwick/fenwick.hpp"

int main()
{
        Fenwick<int64_t> fw(5);
        fw.add(0, 1);
        fw.add(2, 3);
        fw.add(4, 5);
        assert(fw.sum(0, 4) == 9);
        assert(fw.sum(1, 3) == 3);
        assert(fw.lower_bound(4) == 2);

        RangeFenwick<int64_t> rfw(5);
        rfw.range_add(1, 3, 2);
        rfw.range_add(2, 4, 1);
        assert(rfw.point_query(1) == 2);
        assert(rfw.point_query(2) == 3);
        assert(rfw.range_sum(0, 4) == 9);
        cout << "Hello World" << endl;
        return 0;
}

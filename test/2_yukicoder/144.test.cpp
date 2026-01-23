#define PROBLEM "https://yukicoder.me/problems/no/144"
#define ERROR 1e-6
#include "nt/sieve.hpp"
#include <bits/stdc++.h>

using namespace std;

void solve() {
        int N;
        long double p, ans = 0.0;
        cin >> N >> p;
        NT::Sieve sieve(1000000);
        for (int i = 2; i <= N; i++) {
                if (sieve.is_prime(i)) {
                        ans += 1.0;
                } else {
                        ans += pow(1 - p, sieve.count_divisors(i) - 2);
                }
        }
        cout << fixed << setprecision(12) << ans << '\n';
}

int main() {
        ios::sync_with_stdio(false);
        cin.tie(NULL);

        int t = 1;
        // cin >> t;
        while (t--) solve();

        return 0;
}

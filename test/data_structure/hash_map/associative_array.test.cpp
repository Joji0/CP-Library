#define PROBLEM "https://judge.yosupo.jp/problem/associative_array"
#include "data_structure/hash_map.hpp"
#include <bits/stdc++.h>
using namespace std;

void solve() {
	int Q;
	cin >> Q;
	// minimize resizing
	HashMap<long long, long long> hash_map(Q);
	while (Q--) {
		int t;
		cin >> t;
		if (!t) {
			long long k, v;
			cin >> k >> v;
			hash_map[k] = v;
		} else {
			long long k;
			cin >> k;
			cout << hash_map[k] << '\n';
		}
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	int t = 1;
	// cin >> t;
	while (t--) solve();

	return 0;
}

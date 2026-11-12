// 我思う、故に我在り
#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"
#include <bits/stdc++.h>
#include "data_structure/fast_set.hpp"

void solve() {
	int N, Q;
	std::cin >> N >> Q;
	FastSet fs(N);
	std::string s;
	std::cin >> s;
	fs.build(' ' + s);
	while (Q--) {
		char c;
		int k;
		std::cin >> c >> k;
		++k;
		if (c == '0') {
			fs.insert(k);
		} else if (c == '1') {
			fs.erase(k);
		} else if (c == '2') {
			if (fs.contains(k))
				std::cout << "1\n";
			else
				std::cout << "0\n";
		} else if (c == '3') {
			const int result = fs.next(k);
			std::cout << (result == -1 ? -1 : result - 1) << '\n';
		} else {
			const int result = fs.prev(k);
			std::cout << (result == -1 ? -1 : result - 1) << '\n';
		}
	}
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int tc = 1;
	// std::cin >> tc;
	while (tc--) solve();
	return 0;
}

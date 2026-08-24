#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>

// Requirement for BlockType:
// using T = ...; (Type of array elements)
// using Ans = ...; (Type of query answer)
// int l, r;
// void init(int _l, int _r, const std::vector<T>& a);
// void build(const std::vector<T>& a);
// void push(std::vector<T>& a);
// void update_partial(int ql, int qr, std::vector<T>& a, Args... args);
// void update_whole(Args... args);
// Ans query_partial(int ql, int qr, const std::vector<T>& a, Args... args);
// Ans query_whole(Args... args);
// static Ans op(Ans a, Ans b);
// static Ans e();

template <typename BlockType>
struct GenericSqrt {
	using T = typename BlockType::T;
	using Ans = typename BlockType::Ans;
	int n, b_size, b_cnt;
	std::vector<T> a;
	std::vector<BlockType> blocks;
	GenericSqrt(const std::vector<T> &init_a, int block_size = -1) {
		n = (int)init_a.size();
		b_size = 1;
		b_cnt = 0;
		if (n == 0) return;
		assert(block_size == -1 || block_size > 0);
		b_size = block_size == -1 ? std::max(1, (int)std::sqrt(n)) : block_size;
		b_cnt = (n + b_size - 1) / b_size;
		a = init_a;
		blocks.resize(b_cnt);
		for (int i = 0; i < b_cnt; i++) {
			int l = i * b_size;
			int r = std::min(n - 1, l + b_size - 1);
			blocks[i].init(l, r, a);
		}
	}
	template <typename... Args>
	void update(int l, int r, Args... args) {
		if (l > r) return;
		assert(0 <= l && l <= r && r < n);
		int bl = l / b_size, br = r / b_size;
		if (bl == br) {
			blocks[bl].push(a);
			blocks[bl].update_partial(l, r, a, args...);
			blocks[bl].build(a);
		} else {
			blocks[bl].push(a);
			blocks[bl].update_partial(l, blocks[bl].r, a, args...);
			blocks[bl].build(a);
			for (int i = bl + 1; i < br; i++) {
				blocks[i].update_whole(args...);
			}
			blocks[br].push(a);
			blocks[br].update_partial(blocks[br].l, r, a, args...);
			blocks[br].build(a);
		}
	}
	template <typename... Args>
	Ans query(int l, int r, Args... args) {
		if (l > r) return BlockType::e();
		assert(0 <= l && l <= r && r < n);
		int bl = l / b_size, br = r / b_size;
		if (bl == br) {
			blocks[bl].push(a);
			return blocks[bl].query_partial(l, r, a, args...);
		} else {
			blocks[bl].push(a);
			Ans res = blocks[bl].query_partial(l, blocks[bl].r, a, args...);
			for (int i = bl + 1; i < br; i++) {
				res = BlockType::op(res, blocks[i].query_whole(args...));
			}
			blocks[br].push(a);
			res = BlockType::op(res, blocks[br].query_partial(blocks[br].l, r, a, args...));
			return res;
		}
	}
	void push_all() {
		for (int i = 0; i < b_cnt; i++) blocks[i].push(a);
	}
};

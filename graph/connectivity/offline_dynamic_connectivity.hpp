#pragma once
#include <cassert>
#include <cstddef>
#include <numeric>
#include <utility>
#include <vector>

// Time is 1-based. An edge is present from its add time up to, but excluding, its remove time.
class OfflineDynamicConnectivity {
	struct EdgeLifetime {
		int first, second, start, finish = 0;
	};
	int times;
	std::vector<EdgeLifetime> edges{{0, 0, 0, 0}};

	static void schedule(std::vector<std::vector<int>> &events, std::size_t node, int left, int right,
			     int start, int finish, int id) {
		if (start <= left && right <= finish) {
			events[node].push_back(id);
			return;
		}
		const int middle = std::midpoint(left, right);
		if (start <= middle) schedule(events, node * 2, left, middle, start, finish, id);
		if (middle < finish) schedule(events, node * 2 + 1, middle + 1, right, start, finish, id);
	}

       public:
	explicit OfflineDynamicConnectivity(int times) : times(times) { assert(times >= 0); }

	int add_edge(int time, int first, int second) {
		assert(1 <= time && time <= times && first >= 1 && second >= 1);
		edges.push_back({first, second, time});
		return int(edges.size() - 1);
	}
	void remove_edge(int time, int id) {
		assert(1 <= id && std::size_t(id) < edges.size());
		auto &edge = edges[id];
		assert(edge.start <= time && time <= times && edge.finish == 0);
		edge.finish = time;
	}

	// State must provide snapshot() and rollback(). The visitor observes the state at each time.
	template <typename State, typename ApplyEdge, typename Visit>
	void execute(State &state, const ApplyEdge &apply_edge, const Visit &visit) const {
		if (times == 0) return;
		std::vector<std::vector<int>> events(4 * std::size_t(times));
		for (std::size_t id = 1; id < edges.size(); ++id) {
			const auto &edge = edges[id];
			const int finish = edge.finish == 0 ? times : edge.finish - 1;
			if (edge.start <= finish) schedule(events, 1, 1, times, edge.start, finish, int(id));
		}
		const auto traverse = [&](const auto &self, std::size_t node, int left, int right) -> void {
			const auto previous = state.snapshot();
			try {
				for (int id : events[node]) apply_edge(state, edges[id].first, edges[id].second);
				if (left == right) {
					visit(left, std::as_const(state));
				} else {
					const int middle = std::midpoint(left, right);
					self(self, node * 2, left, middle);
					self(self, node * 2 + 1, middle + 1, right);
				}
			} catch (...) {
				state.rollback(previous);
				throw;
			}
			state.rollback(previous);
		};
		traverse(traverse, 1, 1, times);
	}

	template <typename State, typename Visit>
	void execute(State &state, const Visit &visit) const {
		execute(state, [](State &current, int first, int second) {
			if constexpr (requires { current.merge(first, second); })
				current.merge(first, second);
			else
				current.add_edge(first, second); }, visit);
	}
};

#pragma once

#include <functional>

class KState {
	int state;
public:
	const int size;
	KState(int size, int state = 0);
	KState& operator=(KState const& b);

	bool get(int index) const;
	void set(int index);
	void set(int index, bool value);
	void clr(int index);
	void toggle(int index);

	bool operator==(KState const& b) const;
	bool operator!=(KState const& b) const;
	size_t hash() const;
};

template<>
struct std::hash<KState> {
	size_t operator()(const KState& state) const noexcept {
		return state.hash();
	}
};

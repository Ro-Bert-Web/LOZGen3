#pragma once

#include "KVal.h"
#include "KState.h"

#include <vector>
#include <functional>

class KBox {
	std::vector<KVal> state;

public:
	const int size;

	KBox(int size);
	KBox(KState state);
	KBox& operator=(KBox const& b);

	KVal get(int index) const;
	void set(int index, KVal value);
	bool contains(KBox const& b) const;

	int specificity() const;

	bool operator==(KBox const& b) const;
	bool operator!=(KBox const& b) const;
	size_t hash() const;
};

template<>
struct std::hash<KBox> {
	size_t operator()(const KBox& box) const noexcept {
		return box.hash();
	}
};

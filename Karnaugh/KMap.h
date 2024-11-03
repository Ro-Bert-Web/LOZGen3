#pragma once

#include <unordered_map>
#include <unordered_set>
#include "KVal.h"
#include "KState.h"
#include "KBox.h"

typedef std::unordered_set<KState> KSet;

class KMap {
	std::unordered_map<KVal, KSet> sets;
	KVal defacto;

	int maxNumberOfStates() const;

public:
	const int size;

	KMap(int size, KVal defacto = X);
	KMap(KBox box);

	void defact(KVal value);

	KVal get(KState state) const;
	void set(KState state, KVal value);
	KSet get(KVal value) const;

	int numberOfStates(KVal value) const;

	KMap& operator=(KMap const& b);
	KMap& operator&=(KMap const& b);
	KMap& operator|=(KMap const& b);
	KMap& operator^=(KMap const& b);

	KMap operator~() const;
	KMap operator&(KMap const& b) const;
	KMap operator|(KMap const& b) const;
	KMap operator^(KMap const& b) const;

	KMap& invert(int index);
	KMap given(KBox const& box) const;

	bool operator==(KMap const& b) const;
	bool operator!=(KMap const& b) const;
	bool empty() const;
	bool zero() const;

	KMap exact() const;
	KMap maybe() const;

	std::unordered_set<KBox> solve() const;
	void resolve();
};

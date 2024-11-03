#include "KMap.h"
#include <stdexcept>

KMap::KMap(int size, KVal defacto) : size{ size }, defacto{ defacto } {
	if (defacto != T)
		sets.emplace(T, KSet());
	if (defacto != F)
		sets.emplace(F, KSet());
	if (defacto != X)
		sets.emplace(X, KSet());
}

KMap::KMap(KBox box) : size{ box.size }, defacto{ F } {
	sets.emplace(T, KSet());
	sets.emplace(X, KSet());

	for (int i = 0; i < maxNumberOfStates(); i++) {
		KState state(size, i);
		if (box.contains(state))
			set(state, T);
	}
}

// Set management
int KMap::maxNumberOfStates() const {
	return 1 << size;
}

int KMap::numberOfStates(KVal value) const {
	auto found = sets.find(value);
	if (found != sets.end()) {
		return (int)found->second.size();
	}
	else {
		int rslt = maxNumberOfStates();
		for (auto it = sets.begin(); it != sets.end(); it++)
			rslt -= (int)it->second.size();
		return rslt;
	}
}

void KMap::defact(KVal value) {
	if (defacto == value)
		return;

	std::unordered_map<KVal, KSet> newSets;

	KSet visited;
	for (auto it = sets.begin(); it != sets.end(); it++) {
		if (it->first != value)
			newSets.emplace(it->first, it->second);
		visited.insert(it->second.begin(), it->second.end());
	}

	newSets.emplace(defacto, KSet());
	auto found = newSets.find(defacto);
	for (int i = 0; i < maxNumberOfStates(); i++) {
		KState state(size, i);
		if (visited.count(state) == 0)
			found->second.insert(state);
	}

	defacto = value;
	sets = newSets;
}

// Modify by state
KVal KMap::get(KState state) const {
	if (size != state.size)
		throw std::length_error("Sizes differ");

	for (auto it = sets.begin(); it != sets.end(); it++)
		if (it->second.count(state) > 0)
			return it->first;
	return defacto;
}

void KMap::set(KState state, KVal value) {
	if (size != state.size)
		throw std::length_error("Sizes differ");

	for (auto it = sets.begin(); it != sets.end(); it++)
		it->second.erase(state);

	auto found = sets.find(value);
	if (found != sets.end())
		found->second.insert(state);
}

KSet KMap::get(KVal value) const {
	auto found = sets.find(value);
	if (found != sets.end())
		return found->second;

	KSet rslt;
	for (int i = 0; i < maxNumberOfStates(); i++) {
		KState state(size, i);

		if (get(state) == value)
			rslt.insert(state);
	}
	return rslt;
}

// Operators
KMap& KMap::operator=(KMap const& b) {
	if (size != b.size)
		throw std::length_error("Sizes differ");

	defacto = b.defacto;
	sets = b.sets;

	// Maybe re defact if the amounts are lopsided

	return *this;
}

KMap& KMap::operator&=(KMap const& b) {
	if (size != b.size)
		throw std::length_error("Sizes differ");
	return operator=(operator&(b));
}

KMap& KMap::operator|=(KMap const& b) {
	if (size != b.size)
		throw std::length_error("Sizes differ");
	return operator=(operator|(b));
}

KMap& KMap::operator^=(KMap const& b) {
	if (size != b.size)
		throw std::length_error("Sizes differ");
	return operator=(operator^(b));
}

KMap KMap::operator~() const {
	KMap rslt(size, ~defacto);

	for (auto source = sets.begin(); source != sets.end(); source++) {
		auto target = rslt.sets.find(~(source->first));
		target->second = source->second;
	}

	return rslt;
}

KMap KMap::operator&(KMap const& b) const {
	if (size != b.size)
		throw std::length_error("Sizes differ");

	KMap rslt(size, defacto);
	for (int i = 0; i < maxNumberOfStates(); i++) {
		KState state(size, i);
		rslt.set(state, get(state) & b.get(state));
	}
	return rslt;
}

KMap KMap::operator|(KMap const& b) const {
	if (size != b.size)
		throw std::length_error("Sizes differ");

	KMap rslt(size, defacto);
	for (int i = 0; i < maxNumberOfStates(); i++) {
		KState state(size, i);
		rslt.set(state, get(state) | b.get(state));
	}
	return rslt;
}

KMap KMap::operator^(KMap const& b) const {
	if (size != b.size)
		throw std::length_error("Sizes differ");

	KMap rslt(size, defacto);
	for (int i = 0; i < maxNumberOfStates(); i++) {
		KState state(size, i);
		rslt.set(state, get(state) ^ b.get(state));
	}
	return rslt;
}

KMap& KMap::invert(int index) {
	for (auto it = sets.begin(); it != sets.end(); it++) {
		KSet newSet;
		for (auto jt = it->second.begin(); jt != it->second.end(); jt++) {
			KState newState = *jt;
			newState.toggle(index);
			newSet.insert(newState);
		}
		it->second = newSet;
	}
	return *this;
}

KMap KMap::given(KBox const& box) const {
	KMap rslt(size - box.specificity(), F);

	for (int i = 0; i < maxNumberOfStates(); i++) {
		KState state(size, i);

		if (box.contains(state)) {
			KState newState(size - box.specificity());
			for (int j = 0, k = 0; j < size; j++) {
				if (box.get(j) == X) {
					newState.set(k, state.get(j));
					k++;
				}
			}

			rslt.set(newState, get(state));
		}
	}

	return rslt;
}

// Conditionals
bool KMap::operator==(KMap const& b) const {
	if (size != b.size)
		return false;

	for (int i = 0; i < maxNumberOfStates(); i++) {
		KState state(size, i);
		if (get(state) != b.get(state))
			return false;
	}

	return true;
}

bool KMap::operator!=(KMap const& b) const {
	return !operator==(b);
}

bool KMap::empty() const {
	return numberOfStates(T) == 0;
}

bool KMap::zero() const {
	return (numberOfStates(T) + numberOfStates(X)) == 0;
}

KMap KMap::exact() const {
	if (defacto == X || defacto == F) {
		KMap rslt(size, F);
		auto source = sets.find(T);
		auto target = rslt.sets.find(T);

		target->second.insert(source->second.begin(), source->second.end());
		return rslt;
	}
	else {
		KMap rslt = *this;
		rslt.defact(F);
		//rslt.sets.erase(X);
		//rslt.sets.emplace(X, KSet());
		rslt.sets[X] = KSet();

		return rslt;
	}
}

KMap KMap::maybe() const {
	if (defacto == X || defacto == T) {
		KMap rslt(size, T);
		auto source = sets.find(F);
		auto target = rslt.sets.find(F);

		target->second.insert(source->second.begin(), source->second.end());
		return rslt;
	}
	else {
		KMap rslt = *this;
		rslt.defact(T);
		//rslt.sets.erase(X);
		//rslt.sets.emplace(X, KSet());
		rslt.sets[X] = KSet();

		return rslt;
	}
}


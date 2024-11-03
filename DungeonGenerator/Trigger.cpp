#include "Trigger.h"
#include <stdexcept>

Trigger::Trigger(TriggerAttributes attributes, int index) : attributes{ attributes }, index{ index } {}

bool Trigger::canToggleFrom(KState state) const {
	return !state.get(index) || attributes.toggleable;
}

bool Trigger::symmetricAcross(KState state, int index, bool open) const {
	if (open) {
		return index == this->index && attributes.symmetrical ||
			!state.get(this->index) && !attributes.closeable;
	}
	else {
		return index == this->index && attributes.symmetrical ||
			state.get(this->index) && !attributes.closeable;
	}
}

KMap Trigger::enforce(KMap map, int index) const {
	if (index == this->index && attributes.symmetrical) {
		KMap inverse = map;
		inverse.invert(this->index);

		KMap _and = map & inverse;
		KMap _or = map | inverse;
		KMap _xor = map ^ inverse;

		if (!_xor.empty())
			throw std::runtime_error("This map was non enforceable");

		map = _or.exact() | KMap(map.size, X) & _and;
	}

	if (!attributes.closeable) {
		KBox posBox(map.size);
		posBox.set(this->index, T);
		KBox negBox(map.size);
		negBox.set(this->index, F);

		KMap inverse = map;
		inverse.invert(this->index);

		// error if negative of map is T and positive of map is F
		if (!(map.exact() & (~inverse).exact() & negBox).empty())
			throw std::runtime_error("This map was non enforceable");

		KMap pos = (map | inverse.exact()) & posBox;
		KMap neg = (map & inverse.maybe()) & negBox;

		map = pos | neg;
	}

	return map;
}

bool Trigger::operator==(Trigger const& trigger) const {
	return attributes.toggleable == trigger.attributes.toggleable &&
		attributes.symmetrical == trigger.attributes.symmetrical &&
		attributes.multiuse == trigger.attributes.multiuse &&
		attributes.closeable == trigger.attributes.closeable &&
		index == trigger.index;
}

size_t Trigger::hash() const {
	size_t hash = 0;
	hash += attributes.toggleable ? 0b1 : 0;
	hash += attributes.symmetrical ? 0b10 : 0;
	hash += attributes.multiuse ? 0b100 : 0;
	hash += attributes.closeable ? 0b1000 : 0;
	hash = std::hash<size_t>()(hash);
	hash ^= std::hash<int>()(index);
	return hash;
}
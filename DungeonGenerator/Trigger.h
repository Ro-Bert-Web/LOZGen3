#pragma once

#include "KState.h"
#include "KMap.h"

struct TriggerAttributes {
	bool toggleable;
	bool symmetrical;
	bool multiuse;
	bool closeable;
};

const TriggerAttributes Switch = { true, true, true, true };
const TriggerAttributes SwitchAsym = { true, false, true, true };
const TriggerAttributes Key = { false, true, false, false };
const TriggerAttributes Item = { false, true, true, false };
const TriggerAttributes Cataclysm = { false, true, true, true };

class Trigger {
public:
	const TriggerAttributes attributes;
	const int index;
	Trigger(TriggerAttributes attributes, int index);

	bool canToggleFrom(KState state) const;
	bool symmetricAcross(KState state, int index, bool open) const;
	KMap enforce(KMap map, int index) const;

	bool operator==(Trigger const& trigger) const;
	size_t hash() const;
};

template<>
struct std::hash<Trigger> {
	size_t operator()(Trigger const& trigger) const noexcept {
		return trigger.hash();
	}
};

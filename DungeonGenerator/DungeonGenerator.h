#pragma once

#include <vector>
#include <list>
#include "Trigger.h"

#include <cstdlib>

class DungeonGenerator {
	std::vector<Trigger> triggers;
	std::vector<KMap> doors;

	void setDoor(KMap const& visited, KState const& state, int index, bool open);
	void markReachable(KMap& visited, KState const& state);
	bool tryTraverseDoor(KMap& visited, KState& state, Trigger const& trigger);

public:
	DungeonGenerator();
	void addTrigger(TriggerAttributes trigConfig);
	void generate();
	void print();
};

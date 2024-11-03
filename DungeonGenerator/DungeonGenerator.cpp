#include "DungeonGenerator.h"

#include <unordered_set>

#include "KMap.h"
#include "NPerm.h"


#include <stdexcept>
#include <iostream>

template<>
struct std::hash<std::pair<KState, Trigger>> {
	size_t operator()(std::pair<KState, Trigger> const& pair) const noexcept {
		return pair.first.hash() ^ pair.second.hash();
	}
};

DungeonGenerator::DungeonGenerator() {}

void DungeonGenerator::addTrigger(TriggerAttributes trigConfig) {
	triggers.push_back(Trigger(trigConfig, triggers.size()));
}

void DungeonGenerator::setDoor(KMap const& visited, KState const& state, int index, bool open) {
	std::unordered_set<KState> projectedDoors = { state };
	std::list<KState> symmetryCheck = { state };

	while (!symmetryCheck.empty()) {
		KState checkState = symmetryCheck.front();
		symmetryCheck.pop_front();

		for (int i = 0; i < triggers.size(); i++) {
			if (triggers[i].symmetricAcross(checkState, index, open)) {
				KState mirrorState = checkState;
				mirrorState.toggle(triggers[i].index);
				if (projectedDoors.count(mirrorState) == 0) {
					projectedDoors.insert(mirrorState);
					symmetryCheck.push_back(mirrorState);
				}
			}
		}
	}

	for (auto it = projectedDoors.begin(); it != projectedDoors.end(); it++)
		doors[index].set(*it, open ? T : F);
}

void DungeonGenerator::markReachable(KMap& visited, KState const& state) {
	for (int i = 0; i < triggers.size(); i++) {
		if (!triggers[i].canToggleFrom(state))
			continue;
		if (doors[triggers[i].index].get(state) != F)
			continue;

		KState nextState = state;
		if (visited.get(nextState) == F) {
			visited.set(nextState, X);
			markReachable(visited, nextState);
		}
	}
}

bool DungeonGenerator::tryTraverseDoor(KMap& visited, KState& state, Trigger const& trigger) {
	if (!trigger.canToggleFrom(state))
		return false;

	KState nextState = state;
	nextState.toggle(trigger.index);

	if (visited.get(nextState) == T)
		return false;

	if (visited.get(nextState) == X && doors[trigger.index].get(state) != T)
		return false;

	setDoor(visited, state, trigger.index, true);
	state = nextState;
	visited.set(state, T);
	markReachable(visited, state);

	return true;
}

void DungeonGenerator::generate() {
	std::vector<Trigger> toggles;
	for (int i = 0; i < triggers.size(); i++)
		if (triggers[i].attributes.toggleable)
			toggles.push_back(triggers[i]);

	doors = std::vector<KMap>(triggers.size() + 1, KMap(triggers.size(), F));

	KMap visited(triggers.size(), F);
	KState state(triggers.size());
	visited.set(state, T);

	std::list<int> permutation = nperm(rand() % fact(triggers.size()), triggers.size());
	std::unordered_set<Trigger> usedToggles;

	for (auto it = permutation.begin(); it != permutation.end(); it++) {
		int numToggles = std::rand() % (2 * toggles.size() - 1);

		// If trigger is a toggle, that just tells us that if it hasn't been used yet, this round of toggles starts with it
		// Once that's been confirmed, run the rest of the round of toggles, and then pick up on the next trigger
		while (triggers[*it].attributes.toggleable) {
			if (usedToggles.count(triggers[*it]) == 0) {
				if (!tryTraverseDoor(visited, state, triggers[*it]))
					throw std::runtime_error("Somehow, toggle was not traversable");
				usedToggles.insert(triggers[*it]);
				numToggles--;
			}
			it++;
		}

		// Run through toggles
		while (numToggles > 0) {
			std::list<int> tryToggles = nperm(rand() % fact(toggles.size()), toggles.size());

			bool success = false;
			for (auto jt = tryToggles.begin(); jt != tryToggles.end(); jt++) {
				if (tryTraverseDoor(visited, state, triggers[*jt])) {
					success = true;
					break;
				}
			}

			if (success)
				numToggles--;
			else
				throw std::runtime_error("Somehow, no toggle was traversable");
		}

		if (!tryTraverseDoor(visited, state, triggers[*it]))
			throw std::runtime_error("Somehow, trigger was not traversable");
	}
	setDoor(visited, state, triggers.size(), true);

	for (int i = 0; i < triggers.size(); i++) {
		KMap mustBeTrue = doors[triggers[i].index];
		KMap doesntMatter = (~visited).exact() & KMap(triggers.size(), X);
		KMap door = mustBeTrue | doesntMatter;

		for (int j = 0; j < triggers.size(); j++)
			door = triggers[j].enforce(door, triggers[i].index);

		doors[triggers[i].index] = door;
	}
	KMap mustBeTrue = doors[triggers.size()];
	KMap doesntMatter = (~visited).exact();
	doors[triggers.size()] = mustBeTrue | KMap(triggers.size(), X) & doesntMatter;
}

void DungeonGenerator::print() {
	std::cout << "Truth Tables" << std::endl;
	for (auto it = triggers.begin(); it != triggers.end(); it++) {
		KBox boxPos(triggers.size());
		boxPos.set(it->index, T);
		KBox boxNeg(triggers.size());
		boxNeg.set(it->index, F);

		if (it->attributes.symmetrical) {
			std::cout << it->index << " (";
			std::putchar('A' + it->index);
			std::cout << ") = ";
			std::unordered_set<KBox> solution = doors[it->index].solve();
			for (auto jt = solution.begin(); jt != solution.end(); jt++) {
				if (jt->specificity() == 0) {
					std::cout << "1 ";
				}

				for (int i = 0; i < triggers.size(); i++) {
					if (jt->get(i) == T) {
						std::putchar('A' + i);
					}
					else if (jt->get(i) == F) {
						std::putchar('a' + i);
					}
				}
				std::cout << " ";
			}
			std::cout << std::endl;
		}
		else {
			std::cout << it->index << " (";
			std::putchar('A' + it->index);
			std::cout << "+) = ";
			KMap door = doors[it->index] & boxNeg | (doors[it->index] & boxNeg).invert(it->index);

			for (int i = 0; i < triggers.size(); i++)
				door = triggers[i].enforce(door, it->index);

			std::unordered_set<KBox> solution = door.solve();
			for (auto jt = solution.begin(); jt != solution.end(); jt++) {
				if (jt->specificity() == 0) {
					std::cout << "1 ";
				}

				for (int i = 0; i < triggers.size(); i++) {
					if (jt->get(i) == T) {
						std::putchar('A' + i);
					}
					else if (jt->get(i) == F) {
						std::putchar('a' + i);
					}
				}
				std::cout << " ";
			}
			std::cout << std::endl;

			std::cout << "  (";
			std::putchar('A' + it->index);
			std::cout << "-) = ";
			door = doors[it->index] & boxPos | (doors[it->index] & boxPos).invert(it->index);

			for (int i = 0; i < triggers.size(); i++)
				door = triggers[i].enforce(door, it->index);

			solution = door.solve();
			for (auto jt = solution.begin(); jt != solution.end(); jt++) {
				if (jt->specificity() == 0) {
					std::cout << "1 ";
				}

				for (int i = 0; i < triggers.size(); i++) {
					if (jt->get(i) == T) {
						std::putchar('A' + i);
					}
					else if (jt->get(i) == F) {
						std::putchar('a' + i);
					}
				}
				std::cout << " ";
			}
			std::cout << std::endl;
		}
	}
	std::cout << "Finish = ";
	std::unordered_set<KBox> solution = doors[triggers.size()].solve();
	for (auto it = solution.begin(); it != solution.end(); it++) {
		if (it->specificity() == 0) {
			std::cout << "1 ";
		}

		for (int i = 0; i < triggers.size(); i++) {
			if (it->get(i) == T) {
				std::putchar('A' + i);
			}
			else if (it->get(i) == F) {
				std::putchar('a' + i);
			}
		}
		std::cout << " ";
	}
	std::cout << std::endl;
}

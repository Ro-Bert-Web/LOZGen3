#include "KMap.h"

std::unordered_set<KBox> KMap::solve() const {
	std::unordered_set<KBox> finalBoxes;

	KMap truths = exact();

	std::unordered_set<KBox> activeBoxes; {
		KSet unknowns = maybe().get(KVal(T));
		for (auto it = unknowns.begin(); it != unknowns.end(); it++)
			activeBoxes.insert(KBox(*it));
	}

	while (activeBoxes.size() > 0) {
		std::unordered_set<KBox> nextActiveBoxes;

		for (auto it = activeBoxes.begin(); it != activeBoxes.end(); it++) {
			bool foundAdjacent = false;
			for (int i = 0; i < size; i++) {
				if (it->get(i) == F) {
					KBox adjacent = *it;
					adjacent.set(i, T);

					if (activeBoxes.count(adjacent) > 0) {
						adjacent.set(i, X);
						nextActiveBoxes.insert(adjacent);
						foundAdjacent = true;
					}
				}
			}
			if (!foundAdjacent && !(KMap(*it) & truths).empty()) {
				finalBoxes.insert(*it);
			}
		}
		activeBoxes = nextActiveBoxes;
	}

	while (true) {
		// Remove most specific box that adds nothing
		KBox remove(size);
		int specificity = -1;

		for (auto it = finalBoxes.begin(); it != finalBoxes.end(); it++) {
			if (it->specificity() <= specificity)
				continue;

			KMap without(size, F);
			for (auto jt = finalBoxes.begin(); jt != finalBoxes.end(); jt++) {
				if (jt == it)
					continue;
				without |= KMap(*jt) & truths;
			}

			if ((KMap(*it) & truths & ~without).empty()) {
				remove = *it;
				specificity = it->specificity();
			}
		}
		if (specificity == -1)
			break;

		finalBoxes.erase(remove);
	}

	return finalBoxes;
}

void KMap::resolve() {
	std::unordered_set<KBox> sol = solve();
	KMap map(size, F);
	for (int i = 0; i < maxNumberOfStates(); i++) {
		KState state(size, i);
		for (auto it = sol.begin(); it != sol.end(); it++) {
			if (it->contains(state)) {
				map.set(state, T);
				break;
			}
		}
	}
	operator=(map);
}

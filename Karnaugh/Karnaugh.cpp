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

/*
std::unordered_set<KBox> KMap::solve() const {
	std::vector<std::unordered_set<KBox>> finalBoxes(size + 1, std::unordered_set<KBox>());

	KSet truths = get(KVal(T));
	std::unordered_set<KBox> activeBoxes; {
		KSet unknowns = get(KVal(X));

		for (auto it = truths.begin(); it != truths.end(); it++)
			activeBoxes.insert(KBox(*it));
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
			if (!foundAdjacent) {
				finalBoxes[it->specificity()].insert(*it);
			}
		}
		activeBoxes = nextActiveBoxes;
	}

	std::unordered_set<KBox> rslt;
	for (int i = 0; i < size; i++) {
		while (finalBoxes[i].size() > 0) {
			// find box in finalBoxes[i] with largest amount of unvisited truths
			KBox bestBox(size);
			int bestNumTruths = 0;
			for (auto it = finalBoxes[i].begin(); it != finalBoxes[i].end(); it++) {
				int numTruths = 0;
				for (auto jt = truths.begin(); jt != truths.end(); jt++)
					if (it->contains(*jt))
						numTruths++;

				if (numTruths > bestNumTruths) {
					bestBox = *it;
					bestNumTruths = numTruths;
				}
			}

			if (bestNumTruths == 0)
				break;

			KSet newTruths;
			for (auto it = truths.begin(); it != truths.end(); it++)
				if (!bestBox.contains(*it))
					newTruths.insert(*it);
			truths = newTruths;

			rslt.insert(bestBox);
			finalBoxes[i].erase(bestBox);
		}
	}

	return rslt;
}
*/

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

#include <iostream>
#include <vector>
#include "DungeonGenerator.h"

int main() {
	std::srand(0);

	DungeonGenerator lozGen;

	/*
	lozGen.addTrigger(Switch);
	lozGen.addTrigger(SwitchAsym);
	lozGen.addTrigger(Key);
	lozGen.addTrigger(Item);
	lozGen.addTrigger(Cataclysm);
	*/

	lozGen.addTrigger(Switch);
	lozGen.addTrigger(Switch);
	lozGen.addTrigger(Key);
	lozGen.addTrigger(Key);
	lozGen.addTrigger(Key);
	lozGen.addTrigger(Item);

	lozGen.generate();

	lozGen.print();
}

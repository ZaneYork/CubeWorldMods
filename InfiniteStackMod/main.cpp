#include "main.h"
#include "CWSDK/cwsdk.h"
#include <stdio.h>

class Mod : GenericMod {
	virtual int OnCheckInventoryFull(cube::Creature* player, cube::Item* item) override {
		// 1: full, 2: not full, 0: origin logic
		return 2;
	}
};

EXPORT Mod* MakeMod() {
	return new Mod();
}

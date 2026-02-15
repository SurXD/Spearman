#ifndef DUNGEON_H
#define DUNGEON_H

#include "floor.h"
#include "hero.h"

class Dungeon {
	int current_floor;
	vector<Floor> floors;
	Hero hero;
public:
	Dungeon();
	int change_state();
	void draw();
};

#endif // DUNGEON_H
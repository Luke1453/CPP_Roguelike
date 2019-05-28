#pragma once

class Item {
public:
	string name = "";
	int level = 0;
	bool full_heal = false;
	int added_healh = 0;
	int heal = 0;
	int attack = 0;
	int price=0;

	enum {
		head,
		chest,
		legs,
		feet,
		hand,
		no_slot
	}slot;

};
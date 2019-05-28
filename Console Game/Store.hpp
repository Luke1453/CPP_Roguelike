#pragma once
class Store {
public:

	int x_pos;
	int y_pos;

	char img = 36;

	TCODList<Item*> store_container;

	TCODConsole* store_con;
	int gui_size_x;
	int gui_size_y;
	bool store_closed = false;

	void render()const;
	void find_spawn_position(const Map& level, int player_x, int player_y);
	void render_store_gui(Player& player);
	void renderBar(int x, int y, int width, const string name, float value, float maxValue, const TCODColor& barColor, const TCODColor& backColor);

	Store(int x, int y);
	virtual ~Store();

	std::pair<string, int> loot_table[6] = {
		{"Toque",30}, //
		{"Hoodie",25}, //
		{"Jeans",25},  //
		{"Socks",35}, //
		{"Sword",25},  //
		{"Meat",50},
	};

	void generate_items();
	void print_store_items();
};

void Store::render_store_gui(Player& player) {

	store_con->setDefaultBackground(TCODColor::black);
	store_con->setDefaultForeground(TCODColor(200, 180, 50));
	store_con->printFrame(0, 1, gui_size_x, gui_size_y - 1, true, TCOD_BKGND_DEFAULT, "Store");
	store_con->printFrame(3, 4, gui_size_x / 2 - 4, gui_size_y - 7, true, TCOD_BKGND_DEFAULT, "Items:");
	store_con->printFrame(gui_size_x / 2 + 1, 14, gui_size_x / 2 - 4, 15, false, TCOD_BKGND_DEFAULT, "Equiped Items:");
	store_con->printFrame(gui_size_x / 2 + 1, 31, gui_size_x / 2 - 4, 56, false, TCOD_BKGND_DEFAULT, "Buy:");


	renderBar(gui_size_x / 2 + 1, 9, gui_size_x / 2 - 4, "Books", player.books, player.books, TCODColor::amber, TCODColor::amber);
	renderBar(gui_size_x / 2 + 1, 4, gui_size_x / 2 - 4, "HP", player.health, player.max_healh, TCODColor::lightRed, TCODColor::darkerRed);

	int shortcut = 'a';
	int y = 6;
	for (auto i : player.container) {

		if (i->slot == Item::no_slot) {
			store_con->print(5, y, "(%c) %s (%d Book).", shortcut, i->name.c_str(), i->price);
		}
		else {
			store_con->print(5, y, "(%c) %s (%d Books).", shortcut, i->name.c_str(), i->price);
		}
		y += 2;
		shortcut++;
	}

	TCOD_key_t key_press;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key_press, NULL);	//cecking for key press

	if (key_press.vk == TCODK_ESCAPE) {

		store_closed = true;
		return;
	}
	else {
		//selling items
		int key_shorcut = 'a';
		for (auto i : player.container) {
			if (key_shorcut == key_press.c && (key_press.lalt == false || key_press.ralt == false)) {	//selling

				player.books += i->price;
				player.container.remove(i);
				delete i;
				break;
			}
			else {
				key_shorcut++;
			}
		}
		//buying items
		key_shorcut = 'a';
		for (auto i : store_container) {
			if (key_shorcut == key_press.c && (key_press.lalt == true || key_press.ralt == true)) {	//selling

				if (player.books >= i->price) {

					player.books -= i->price;
					player.container.push(i);
					store_container.remove(i);
				}
				break;
			}
			else {
				key_shorcut++;
			}
		}
	}

	//printing equipment in frame
	for (auto i : player.equiped) {
		store_con->setDefaultForeground(TCODColor(200, 180, 50));
		if (i->slot == Item::head) {

			store_con->print(gui_size_x / 2 + 3, 17, "Head: %s", i->name.c_str());
		}
		else if (i->slot == Item::chest) {

			store_con->print(gui_size_x / 2 + 3, 19, "Chest: %s", i->name.c_str());
		}
		else if (i->slot == Item::legs) {

			store_con->print(gui_size_x / 2 + 3, 21, "Legs: %s", i->name.c_str());
		}
		else if (i->slot == Item::feet) {

			store_con->print(gui_size_x / 2 + 3, 22, "Feet: %s", i->name.c_str());
		}
		else if (i->slot == Item::hand) {

			store_con->print(gui_size_x / 2 + 3, 24, "Right Hand: %s", i->name.c_str());
		}
	}

	//generating store equipment
	generate_items();
	print_store_items();

	TCODConsole::blit(store_con, 0, 0, gui_size_x, gui_size_y, TCODConsole::root, 5, 5);
}

void Store::renderBar(int x, int y, int width, const string name, float value, float maxValue, const TCODColor& barColor, const TCODColor& backColor) {

	store_con->setDefaultBackground(backColor);
	store_con->rect(x, y, width, 3, false, TCOD_BKGND_SET);
	int bar_level = (int)(value / maxValue * width);
	if (bar_level > 0 && !(value > maxValue)) {
		store_con->setDefaultBackground(barColor);
		store_con->rect(x, y, bar_level, 3, false, TCOD_BKGND_SET);
	}
	//print "health" next to heath bar;
	store_con->setDefaultForeground(TCODColor::white);
	store_con->printEx(x + width / 2, y + 1, TCOD_BKGND_NONE, TCOD_CENTER, "%s : %g/%g", name.c_str(), value, maxValue);
}

void Store::render() const {

	TCODConsole::root->setChar(x_pos, y_pos, img);	//Draw player
	TCODConsole::root->setCharForeground(x_pos, y_pos, TCOD_amber);	//change player color to white
}

void Store::find_spawn_position(const Map& level, int player_x, int player_y) {
	int d = 0;		//dirrection
	int s = 0;		//stepcount
	int rect = std::max(level.x_max, level.y_max);	//lenght of smallest rectangle one can fit in map

	for (int i = 1; i <= (rect - 1); i++)
	{
		for (int j = 0; j < (i < (rect - 1) ? 2 : 3); j++)
		{
			for (int z = 0; z < s; z++)
			{
				if (!level.isWall(x_pos, y_pos) && !(player_x == x_pos && player_y == y_pos))return;

				switch (d)
				{
				case 0: y_pos = y_pos + 1; break;
				case 1: x_pos = x_pos + 1; break;
				case 2: y_pos = y_pos - 1; break;
				case 3: x_pos = x_pos - 1; break;
				}
			}
			d = (d + 1) % 4;
		}
		s = s + 1;
	}
}

Store::Store(int x, int y) {

	//srand(time(NULL));
	x_pos = random_device()() % x;
	y_pos = random_device()() % y;

	gui_size_x = x - 10;
	gui_size_y = y - 10;
	store_con = new TCODConsole(gui_size_x, gui_size_y);
}

Store::~Store() {
}

void Store::generate_items() {

	while (store_container.size() < 25)
	{
		Item* item = new Item;
		int drop_roll = random_device()() % 100;
		int random_item_index = random_device()() % 5;

		if (drop_roll <= loot_table[random_item_index].second) {

			if (random_item_index == 0) {
				item->slot = Item::head;
				item->level = random_device()() % 5 + 1;
				item->name = loot_table[random_item_index].first + " +" + (char(item->level + 48));
				item->added_healh = item->level * 5;
				item->price = 3 + item->level + (random_device()() % 3);
			}
			else if (random_item_index == 3) {
				item->slot = Item::feet;
				item->level = random_device()() % 5 + 1;
				item->name = loot_table[random_item_index].first + " +" + (char(item->level + 48));
				item->added_healh = item->level * 5;
				item->price = 3 + item->level + (random_device()() % 3);
			}
			else if (random_item_index == 1) {
				item->slot = Item::chest;
				item->level = random_device()() % 5 + 1;
				item->name = loot_table[random_item_index].first + " +" + (char(item->level + 48));
				item->added_healh = item->level * 10;
				item->full_heal = true;
				item->price = 5 + item->level + (random_device()() % 5);
			}
			else if (random_item_index == 2) {
				item->slot = Item::legs;
				item->level = random_device()() % 5 + 1;
				item->name = loot_table[random_item_index].first + " +" + (char(item->level + 48));
				item->added_healh = item->level * 10;
				item->full_heal = true;
				item->price = 5 + item->level + (random_device()() % 5);
			}
			else if (random_item_index == 4) {
				item->slot = Item::hand;
				item->level = random_device()() % 5 + 1;
				item->name = loot_table[random_item_index].first + " +" + (char(item->level + 48));
				item->added_healh = item->level * 5;
				item->attack = item->level * 5;
				item->price = 5 + item->level + (random_device()() % 5);
			}
		}
		else {

			item->slot = Item::no_slot;
			item->name = loot_table[5].first + " 50 hp";
			item->heal = 50;
			item->price = 2;
		}

		store_container.push(item);
	}
}

void Store::print_store_items() {

	int shortcut = 'a';
	int y = 33;
	for (auto i : store_container) {

		if (i->price == 1) {
			store_con->print(gui_size_x / 2 + 3, y, "(ALT + %c) %s (%d Book).", shortcut, i->name.c_str(), i->price); 
		}
		else {
			store_con->print(gui_size_x / 2 + 3, y, "(ALT + %c) %s (%d Books).", shortcut, i->name.c_str(), i->price);
		}
		y += 2;
		shortcut++;
	}
}

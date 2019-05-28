#pragma once
class Inventory {
public:

	TCODConsole* inv_con;
	int gui_size_x;
	int gui_size_y;

	Item* inplayer;

	void render_inventory_gui(Player& player,int score);
	bool inventry_closed = false;
	void renderBar(int x, int y, int width, const string name, float value, float maxValue, const TCODColor& barColor, const TCODColor& backColor);
	void apply_stat_changes(Player& player, Item& item);
	Inventory(int x, int y);
	virtual ~Inventory();

};

void Inventory::render_inventory_gui(Player& player,int score) {

	
	inv_con->setDefaultBackground(TCODColor::black);
	inv_con->setDefaultForeground(TCODColor(200, 180, 50));
	inv_con->printFrame(0, 1, gui_size_x, gui_size_y - 1, true, TCOD_BKGND_DEFAULT, "Inventory");
	inv_con->printFrame(3, 4, gui_size_x / 2 - 4, gui_size_y - 7, true, TCOD_BKGND_DEFAULT, "Items:");
	inv_con->printFrame(gui_size_x / 2 + 1, 9, gui_size_x / 2 - 4, 15, false, TCOD_BKGND_DEFAULT, "Equiped Items:"); //17 9
	inv_con->printFrame(gui_size_x / 2 + 1, 30, gui_size_x / 2 - 4, 17, false, TCOD_BKGND_DEFAULT, "Stats:"); //17 22
	
	//adding stuff to stat bar;
	inv_con->print(gui_size_x / 2 + 3, 33, "Score: %d", score);
	inv_con->print(gui_size_x / 2 + 3, 35, "Achievement Points: !ADD!");
	inv_con->print(gui_size_x / 2 + 3, 37, "Level: %d", player.level);
	inv_con->print(gui_size_x / 2 + 3, 39, "XP: %d", player.level*30+player.xp-30);
	inv_con->print(gui_size_x / 2 + 3, 41, "Books: %d", player.books);
	inv_con->print(gui_size_x / 2 + 3, 43, "Attack Power: %d", player.attack);


	renderBar(gui_size_x / 2 + 1, 4, gui_size_x / 2 - 4, "HP", player.health, player.max_healh, TCODColor::lightRed, TCODColor::darkerRed);

	int shortcut = 'a';
	int y = 6;
	for (auto i : player.container) {
		inv_con->print(5, y, "(%c) %s", shortcut, i->name.c_str());
		y += 2;
		shortcut++;
	}

	TCOD_key_t key_press;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_RELEASE, &key_press, NULL);	//cecking for key press

	if (key_press.vk == TCODK_ESCAPE) {
		inventry_closed = true;
		//TCODConsole::flush();
		return;
	}
	else {
		int key_shorcut = 'a';
		inv_con->setDefaultForeground(TCODColor::white);
		for (auto i : player.container) {
			if (key_shorcut == key_press.c) {
				if (i->slot == Item::no_slot) {
					if (i->heal + player.health >= player.max_healh) {
						player.health = player.max_healh;
					}
					else {
						player.health += i->heal;
					}
					player.container.remove(i);
					delete i;
					break;
				}
				else {
					apply_stat_changes(player, *i);
					inplayer = i;
					player.equiped.push(inplayer);
				}
				player.container.remove(i);
				break;
			}
			else {
				key_shorcut++;
			}
		}
	}

	//printing equipment in frame
	for (auto i : player.equiped) {
		inv_con->setDefaultForeground(TCODColor(200, 180, 50));
		if (i->slot == Item::head) {

			inv_con->print(gui_size_x / 2 + 3, 12, "Head: %s", i->name.c_str());
		}
		else if (i->slot == Item::chest) {

			inv_con->print(gui_size_x / 2 + 3, 14, "Chest: %s", i->name.c_str());
		}
		else if (i->slot == Item::legs) {
		
			inv_con->print(gui_size_x / 2 + 3, 16, "Legs: %s", i->name.c_str());
		}
		else if (i->slot == Item::feet) {
		
			inv_con->print(gui_size_x / 2 + 3, 18, "Feet: %s", i->name.c_str());
		}
		else if (i->slot == Item::hand) {
		
			inv_con->print(gui_size_x / 2 + 3, 20, "Right Hand: %s", i->name.c_str());
		}
	}
	TCODConsole::blit(inv_con, 0, 0, gui_size_x, gui_size_y, TCODConsole::root, 5, 5);
}

void Inventory::renderBar(int x, int y, int width, const string name, float value, float maxValue, const TCODColor& barColor, const TCODColor& backColor) {

	inv_con->setDefaultBackground(backColor);
	inv_con->rect(x, y, width, 3, false, TCOD_BKGND_SET);
	int bar_level = (int)(value / maxValue * width);
	if (bar_level > 0 && !(value > maxValue)) {
		inv_con->setDefaultBackground(barColor);
		inv_con->rect(x, y, bar_level, 3, false, TCOD_BKGND_SET);
	}
	//print "health" next to heath bar;
	inv_con->setDefaultForeground(TCODColor::white);
	inv_con->printEx(x + width / 2, y + 1, TCOD_BKGND_NONE, TCOD_CENTER, "%s : %g/%g", name.c_str(), value, maxValue);
}

void Inventory::apply_stat_changes(Player& player, Item& item){

	player.max_healh += item.added_healh;
	player.health += item.heal;
	player.attack += item.attack;
	if (item.full_heal) {
		player.health = player.max_healh;
	}
}

Inventory::Inventory(int x, int y) {
	gui_size_x = x - 10;
	gui_size_y = y - 10;
	inv_con = new TCODConsole(gui_size_x, gui_size_y);
	inplayer = new Item();
}

Inventory::~Inventory() {

}

#pragma once
class Enemy {
public:

	int lvl;
	int health;
	int attack;
	//position
	int x;
	int y;

	TCODColor color = TCODColor::green;
	unsigned char img = 88;		//Enemy symbol

	void render()const;			//rendering function
	void find_spawn_position(const Map& level, int player_x, int player_y);	//finding spawn possition

	Enemy(int max_x, int max_y);
	virtual ~Enemy();

	std::pair<string, int> loot_table[6] = {
		{"Toque",30}, //
		{"Hoodie",25}, //
		{"Jeans",25},  //
		{"Socks",35}, //
		{"Sword",25},  //
		{"Meat",50},
	};

	Item *generate_drop(Item* drop);
};

void Enemy::render() const {
	TCODConsole::root->setChar(x, y, img);	//Draw player
	TCODConsole::root->setCharForeground(x, y, (color * ((float)lvl * 0.4f)));	//change player color to white
}

void Enemy::find_spawn_position(const Map& level,int player_x, int player_y) {
	int d = 0;		//dirrection
	int s = 0;		//stepcount
	int rect = std::max(level.x_max, level.y_max);	//lenght of smallest rectangle one can fit in map

	for (int i = 1; i <= (rect - 1); i++)
	{
		for (int j = 0; j < (i < (rect - 1) ? 2 : 3); j++)
		{
			for (int z = 0; z < s; z++)
			{
				if (!level.isWall(x, y) && !(player_x == x && player_y == y))return;

				switch (d)
				{
				case 0: y = y + 1; break;
				case 1: x = x + 1; break;
				case 2: y = y - 1; break;
				case 3: x = x - 1; break;
				}
			}
			d = (d + 1) % 4;
		}
		s = s + 1;
	}
}

Enemy::Enemy(int max_x, int max_y) {
	//srand(time(NULL));
	Enemy::lvl = random_device()() % 5 + 1;
	Enemy::attack = lvl + 2;
	Enemy::health = random_device()() % 50 + 25;
	Enemy::x = random_device()() % max_x;
	Enemy::y = random_device()() % max_y;
}

Enemy::~Enemy() {
}

Item* Enemy::generate_drop(Item *drop){

	//srand(time(NULL));

	int drop_roll = random_device()() % 100;
	int random_item_index = random_device()() % 5;

	if (drop_roll <= loot_table[random_item_index].second) {

		if (random_item_index == 0 ) {
			drop->slot=Item::head;
			drop->level = random_device()() % 5 + 1;
			drop->name = loot_table[random_item_index].first + " +" + (char(drop->level+48));
			drop->added_healh = drop->level * 5;
			drop->price = 3 + drop->level;
		}
		else if (random_item_index == 3) {
			drop->slot = Item::feet;
			drop->level = random_device()() % 5 + 1;
			drop->name = loot_table[random_item_index].first + " +" + (char(drop->level + 48));
			drop->added_healh = drop->level * 5;
			drop->price = 3 + drop->level;
		}
		else if (random_item_index == 1) {
			drop->slot = Item::chest;
			drop->level = random_device()() % 5 + 1;
			drop->name = loot_table[random_item_index].first + " +" + (char(drop->level + 48));
			drop->added_healh = drop->level * 10;
			drop->full_heal = true;
			drop->price = 5 + drop->level;
		}
		else if (random_item_index == 2) {
			drop->slot = Item::legs;
			drop->level = random_device()() % 5 + 1;
			drop->name = loot_table[random_item_index].first + " +" + (char(drop->level + 48));
			drop->added_healh = drop->level * 10;
			drop->full_heal = true;
			drop->price = 5 + drop->level;
		}
		else if (random_item_index == 4) {
			drop->slot = Item::hand;
			drop->level = random_device()() % 5 + 1;
			drop->name = loot_table[random_item_index].first + " +" + (char(drop->level + 48));
			drop->added_healh = drop->level * 5;
			drop->attack = drop->level * 5;
			drop->price = 5 + drop->level;
		}
	}
	else if (random_device()() % 2 == 1) {

		drop->slot = Item::no_slot;
		drop->name = loot_table[5].first + " 50 hp";
		drop->heal = 50;
		drop->price = 1;
	}
	else {
		
		return NULL;
	}

	return drop;
}
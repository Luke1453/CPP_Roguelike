#pragma once

class Player {
public:

	TCODList<Item*> container;
	TCODList<Item*> equiped;

	//position
	int x;
	int y;
	float health = 100;
	float max_healh = 100;
	int books = 0;				//money
	int xp = 0;
	int level = 1;
	int attack = 10;
	unsigned char img = 65;		//player symbol on screen

	void render()const;			//rendering function
	void find_spawn_position(const Map& level);	//finding spawn possition

	Player(int newX, int newY);	//constructor
	virtual ~Player();			//destructor
};

void Player::render() const {
	TCODConsole::root->setChar(x, y, img);	//Draw player
	TCODConsole::root->setCharForeground(x, y, TCOD_white);	//change player color to white
}

void Player::find_spawn_position(const Map& level) {
	int d = 0;		//dirrection
	int s = 0;		//stepcount
	int rect = std::max(level.x_max, level.y_max);	//lenght of smallest rectangle one can fit in map

	for (int i = 1; i <= (rect - 1); i++)
	{
		for (int j = 0; j < (i < (rect - 1) ? 2 : 3); j++)
		{
			for (int z = 0; z < s; z++)
			{
				if (!level.isWall(x, y))return;

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

Player::Player(int newX, int newY) {

	Player::x = newX;
	Player::y = newY;
}

Player::~Player() {
}
#pragma once

class Map {
public:
	int x_max,						//size x
		y_max;						//size y
	TCODMap* map;					//dungeon map

	bool isWall(int x, int y)const;	//checking if map tile is walkable
	bool is_in_fov(int x, int y)const;	//checking if tile is in field of view      TODO: no way to save tiles i have in fov do i need it?
	void render()const;				//rendering map to the screen
	void draw_fov(int x, int y, int rad);

	Map(int X, int Y);				//constructor
	virtual ~Map();					//destructor
};

bool Map::isWall(int x, int y) const {
	return !map->isWalkable(x, y);
}

bool Map::is_in_fov(int x, int y)const {
	if (map->isInFov(x, y))return true;
}

void Map::render()const {
	static const TCODColor darkWall(25, 25, 25);
	static const TCODColor darkGround(69, 69, 69);
	static const TCODColor lightWall(125, 125, 125);
	static const TCODColor lightGround(169, 169, 169);

	for (int x = 0; x < x_max; x++) {
		for (int y = 0; y < y_max; y++) {
			if (is_in_fov(x, y)) {
				TCODConsole::root->setCharBackground(x, y, isWall(x, y) ? lightWall : lightGround);
			}
			else {
				TCODConsole::root->setCharBackground(x, y, isWall(x, y) ? darkWall : darkGround);
			}
		}
	}
}

void Map::draw_fov(int x, int y, int rad) {
	map->computeFov(x, y, rad,true,FOV_SHADOW);
}

Map::Map(int X, int Y) {
	x_max = X;
	y_max = Y;

	BspHelper bsp;
	map = new TCODMap(x_max, y_max);

	bsp.createBspDungeon(map, NULL);
}

Map::~Map() {
	delete map;
}
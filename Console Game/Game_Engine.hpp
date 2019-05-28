#pragma once
class Game_Engine {
public:
	Player* player;
	Map* level;
	TCODList<Enemy*> enemies;
	Gui* gui;
	Score* score;
	Inventory* inv;
	Store* store;

	string player_name;

	Item* drop;

	enum Game_State {
		STARUP,
		IDLE,
		NEW_TURN,
		VICTORY,
		DEFEAT,
		INVENTORY,
		STORE,
		PAUSE
	}game_state;

	int window_x;
	int window_y;
	int fov_rad = 10;
	void death_handling();
	bool game_exit();	//todo
	void store_handling();
	void inventory_handling();
	bool gameover();
	void update();
	void render();
	bool pl_en_collision(int x, int y);	//player and enemy collision system/atacking
	void enemy_rand_movement();
	Game_Engine(int x_max, int y_max);	//constructor
	virtual ~Game_Engine();				//destructor
};



void Game_Engine::death_handling(){

	//todo
	//TCODConsole::root->clear();

	TCODConsole::root->setDefaultForeground(TCODColor(225, 225, 225));
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			if (((i < 24 || i>75) || (j < 25 || j>55)))
			{
				TCODConsole::root->print(i, j, "~");
			}
		}
	}
	TCODConsole::root->printEx(50, 30, TCOD_BKGND_NONE, TCOD_CENTER, "You died.");
	TCODConsole::root->print(28, 35, "(Enter) Your name: ");
	TCODConsole::root->printFrame(25, 25, 50, 30, false, TCOD_BKGND_DEFAULT, "Dorky Rogue");
	TCODConsole::flush();

	char *letter;
	int letter_x_pos = 46;
	TCOD_key_t key_press;
	//TCOD_event_t ev = TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key_press, NULL, true);
	
		do
		{
			TCOD_event_t ev = TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key_press, NULL, true);
			letter = &key_press.c;
			letter_x_pos++;
			TCODConsole::root->print(letter_x_pos, 35, letter);
			
			TCODConsole::flush();
		} while (key_press.vk != TCODK_ENTER);
	

	TCODConsole::flush();
}
//todo window dosent close


void Game_Engine::store_handling(){

	store->store_closed = false;

	while (!store->store_closed) {

		store->render_store_gui(*player);
		TCODConsole::flush();
	}
	this->render();
	TCODConsole::flush();
	gui->log_message(TCODColor::cyan, "You left the Store!");
}

void Game_Engine::inventory_handling(){

	inv->inventry_closed = false;

	while (!inv->inventry_closed) {

		inv->render_inventory_gui(*player,score->total_score());
		TCODConsole::flush();
	}
	this->render();
	TCODConsole::flush();
	gui->log_message(TCODColor::cyan, "You closed the inventory");
}

bool Game_Engine::gameover(){

	if (game_state == DEFEAT) { return true; }
	return false;
}

bool Game_Engine::game_exit() {

	return false;
}

void Game_Engine::update() {

	if (game_state == STARUP) {
		
		level->draw_fov(player->x, player->y, fov_rad);
		gui->log_message(TCODColor::cyan, "You opened your eyes!", player->level);
		
		game_state = NEW_TURN;
	}
	
	if (game_state == DEFEAT) {
		
		death_handling();
	}
	
	if (game_state == STORE) {

		store_handling();
		game_state = NEW_TURN;
	}

	if (game_state == INVENTORY) {

		inventory_handling();
		game_state = NEW_TURN;
	}

	if (game_state == NEW_TURN) {

		if (player->xp >= 30) {
			
			player->level += 1;
			player->xp -= 30;
			player->max_healh += 5;
			player->health = player->max_healh;
			player->attack += 2;
			gui->log_message(TCODColor::cyan, "You leveled up! You are now level %i and your health was replenished.", player->level);
			gui->log_message(TCODColor::cyan, "You also got +2 attack strenght and +5 max health!");
			score->level_score += player->level * 100;
		}
		game_state = IDLE;
	}

	if (game_state == IDLE){
		//game_state = DEFEAT; //todo change 
		TCOD_key_t key_press; 
		TCOD_event_t ev = TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key_press, NULL, true);

		//Player movement system
		switch (key_press.vk) {

		case TCODK_UP:
			game_state = NEW_TURN;
			if (pl_en_collision(player->x, player->y - 1)) {
				break;
			}
			else if (!level->isWall(player->x, player->y - 1) && !pl_en_collision(player->x, player->y - 1)) {
				player->y--;
				level->draw_fov(player->x, player->y, fov_rad);
			}
			break;

		case TCODK_DOWN:
			game_state = NEW_TURN;
			if (pl_en_collision(player->x, player->y + 1)) {
				break;
			}
			else if (!level->isWall(player->x, player->y + 1) && !pl_en_collision(player->x, player->y + 1)) {
				player->y++;
				level->draw_fov(player->x, player->y, fov_rad);
			}
			break;

		case TCODK_LEFT:
			game_state = NEW_TURN;
			if (pl_en_collision(player->x - 1, player->y)) {
				break;
			}
			else if (!level->isWall(player->x - 1, player->y) && !pl_en_collision(player->x - 1, player->y)) {
				player->x--;
				level->draw_fov(player->x, player->y, fov_rad);
			}
			break;

		case TCODK_RIGHT:
			game_state = NEW_TURN;
			if (pl_en_collision(player->x + 1, player->y)) {
				break;
			}
			else if (!level->isWall(player->x + 1, player->y) && !pl_en_collision(player->x + 1, player->y)) {
				player->x++;
				level->draw_fov(player->x, player->y, fov_rad);
			}
			break;
		default:
			
			if (key_press.c == 'e') {
				game_state = INVENTORY;
			}
			else if (key_press.c == 's' && (player->x==store->x_pos &&player->y==store->y_pos)) {
				game_state = STORE;
			}
			else {
				break;
			}
		}
		enemy_rand_movement();
	}
}

void Game_Engine::render() {

	//if (!game_state == DEFEAT) { TCODConsole::root->clear(); }
	TCODConsole::root->clear();
	level->render();		//Drawing level
	gui->render(player->health,player->max_healh,player->books,player->xp);
	player->render();		//Drawing player

	if (level->is_in_fov(store->x_pos, store->y_pos)) { store->render(); }
	for (const auto e : enemies) {
		if (level->is_in_fov(e->x, e->y)) {
			e->render();

		}
	}
}

bool Game_Engine::pl_en_collision(int px, int py) {
	for (auto e : enemies) {
		if (e->x == px && e->y == py) {
			e->health -= player->attack;
			player->health -= e->attack;

			gui->log_message(TCODColor::white, "You received %i damage and did %i damage to level %i Green Orc.", e->attack, player->attack,e->lvl);

			if (player->health <= 0) {
				gui->log_message(TCODColor::crimson, "YOU DIED!");
				gui->log_message(TCODColor::crimson, "YOU WERE KILLED BY LEVEL %i GREEN ORC AFTER RECEIVING %i DAMAGE!", e->lvl, e->attack);
				game_state = DEFEAT;
			}
			if (e->health <= 0) { 
				
				gui->log_message(TCODColor::crimson, "You killed level %i Green Orc, your final blow did %i damage.",e->lvl,e->attack);

				//money drop
				if (random_device()() % 3 == 0) {
					int books_amount = random_device()() % 8 + 2;
					gui->log_message(TCODColor::amber, "Level %i Green Orc dropped %i books!", e->lvl, books_amount);
					player->books += books_amount;
					score->book_score += books_amount * 5;
				}

				//item drop
				drop = new Item();
				drop = e->generate_drop(drop);
				if (drop == NULL) { 
					delete drop; 
				}
				else {
					
					gui->log_message(TCODColor::fuchsia, "Level %i Green Orc dropped %s", e->lvl, drop->name.c_str());
					player->container.push(drop);
				}

				//add exp
				player->xp += 3;
				score->kill_score += 10;
				score->xp_score += 3;

				enemies.remove(e);
				delete e; 
			}
			return true;
		}
	}
	return false;
}

void Game_Engine::enemy_rand_movement() {
	
	int dir;
	for (auto e : enemies) {
		//srand(time(NULL));
		dir = random_device()() % 4;
		switch (dir) {

		case 0:
			//up
			if (!level->isWall(e->x, e->y - 1) && player->x != e->x && player->y != e->y - 1) {
				e->y--;
			}
			break;
		
		case 1:
			//down
			if (!level->isWall(e->x, e->y + 1) && player->x != e->x && player->y != e->y + 1) {
				e->y++;
			}
			break;
		
		case 2:
			//down
			if (!level->isWall(e->x-1, e->y) && player->x != e->x-1 && player->y != e->y ) {
				e->x--;
			}
			break;
		
		case 3:
			//right
			if (!level->isWall(e->x + 1, e->y) && player->x != e->x + 1 && player->y != e->y) {
				e->x++;
			}
			break;
		
		default: break;
		}
	}
}

Game_Engine::Game_Engine(int x_max, int y_max) : game_state(STARUP),enemies() {

	window_x = x_max;
	window_y = y_max;

	//TCODConsole::initRoot(x_max, y_max, "Dorky Rogue", false, TCOD_RENDERER_SDL);	//Creating game window
	//TCODConsole::setKeyboardRepeat(int initialDelay, int interval);
	
	player = new Player(x_max / 2, y_max / 2);					//Creating player object
	gui = new Gui(x_max, y_max);								//creating additional gui
	level = new Map(x_max, y_max-gui->PANEL_HEIGHT);			//creating level
	player->find_spawn_position(*level);						//find spawn point in walkaple space for player
	score = new Score();
	inv = new Inventory(x_max, y_max);
	store = new Store(x_max, y_max);
	store->find_spawn_position(*level, player->x, player->y);

	
	//generating enemies
	//srand(time(NULL));
	for (int i = 0; i < random_device()() % 25 + 50; i++) {

		enemies.push(new Enemy(x_max, y_max));
	}
	for (const auto e : enemies) { e->find_spawn_position(*level,player->x,player->y); }
}

Game_Engine::~Game_Engine() {
	
	enemies.clearAndDelete();
	delete player;
	delete level;
	delete gui;
	delete store;
	delete inv;
	delete score;
}

//TODO: add one enemy when you kill 3; killscore/multiplier%3->add enemy
//TODO: gameover shit
//TODO: enenies drops; meat =heals
//todo fovrad 10, nim room size 7
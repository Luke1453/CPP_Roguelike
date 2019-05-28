#pragma once
class Menu{
public:
	
	Game_Engine* engine;

	int window_x;
	int window_y;

	void run_game();
	void start_screen();

	Menu(int x_max, int y_max);
	virtual ~Menu();
};



void Menu::start_screen() {

	TCODConsole::root->clear();

	TCODConsole::root->setDefaultForeground(TCODColor(225, 225, 225));
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			if (((i<24||i>75)||(j<25||j>55)))
			{
				TCODConsole::root->print(i, j, "~");
			}
		}
	}

	TCODConsole::root->printEx(50, 30, TCOD_BKGND_NONE, TCOD_CENTER, "(E) Start Game!");
	TCODConsole::root->printEx(50, 35, TCOD_BKGND_NONE, TCOD_CENTER, "(L) Load Game");
	TCODConsole::root->printEx(50, 40, TCOD_BKGND_NONE, TCOD_CENTER, "(D) Leaderboard");
	TCODConsole::root->printEx(50, 45, TCOD_BKGND_NONE, TCOD_CENTER, "(X) Export Leaderboard");
	TCODConsole::root->printEx(50, 50, TCOD_BKGND_NONE, TCOD_CENTER, "(ESC) Exit game");
	TCODConsole::root->printFrame(25, 25, 50, 30, false, TCOD_BKGND_DEFAULT, "Dorky Rogue");
	TCODConsole::flush();

	TCOD_key_t key_press;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key_press, NULL);	//cecking for key press

	if (key_press.vk == TCODK_ESCAPE) {

		exit(0);
	}
	else {

		if (key_press.c == 'e') {
			run_game();
		}
		else if (key_press.c == 'l') {
			//load game
		}
		else if (key_press.c == 'd') {
			//view leader board
		}
		else if (key_press.c == 'x') {
			//export leader board
		}
	}
}

void Menu::run_game(){

	engine = new Game_Engine(window_x, window_y);

	engine->render();
	TCODConsole::flush();

	while (!TCODConsole::isWindowClosed()||!engine->game_exit()) {	//Game loop

		TCODConsole::root->clear();	//Clearing everything from buffer
		engine->update();
		engine->render();
		TCODConsole::flush();		//Updating console window
	}
}

Menu::Menu(int x_max, int y_max){
	
	window_x = x_max;
	window_y = y_max;

	TCODConsole::initRoot(x_max, y_max, "Dorky Rogue", false, TCOD_RENDERER_SDL);
}


Menu::~Menu(){

}

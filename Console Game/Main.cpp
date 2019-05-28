using namespace std;
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstdarg>
#include <utility>
#include <fstream>
#include <random>

#include "libtcod.hpp"
#include "BSPHelper mapgen/bsp_helper.hpp"	// Helper header from libtcod for creating dungeon maps

#include "Item.hpp"
#include "Map.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Gui.hpp"
#include "Score.hpp"
#include "Inventory.hpp"
#include "Store.hpp"
#include "Game_Engine.hpp"
#include "Menu.hpp"

// Hide the console window
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

constexpr auto ConsoleX = 100;
constexpr auto ConsoleY = 100;

Menu menu(ConsoleX, ConsoleY);

int main() {
	

	while (!TCODConsole::isWindowClosed())
	{
		menu.start_screen();
	}

	//system("pause");
	return 0;
}
#pragma once
class Gui {
public:

	TCODConsole* con;

	static const int PANEL_HEIGHT = 19;
	static const int BAR_WIDTH = 28;
	static const int LOG_X = 2;
	static const int LOG_HEIGHT = 10;
	static const int LOG_WIDTH = 86;

	int root_x_max;
	int root_y_max;

	void render(float hp, float max_hp, int books, int xp);
	void renderBar(int x, int y, int width, const string name, float value, float maxValue, const TCODColor& barColor, const TCODColor& backColor);
	void log_message(const TCODColor& col, const string text, ...);

	Gui(int x_max, int y_max);
	virtual ~Gui();

protected:

	struct Message {
		string text;
		TCODColor col;
		Message(const string text, const TCODColor& col);
		//virtual ~Message();								
	};
	TCODList<Message*> log;
};

void Gui::render(float hp, float max_hp, int books, int xp) {
	
	// clear the GUI console 
	con->setDefaultBackground(TCODColor::black);
	con->clear();

	//draving info bars
	renderBar(4, 2, BAR_WIDTH, "HP", hp, max_hp, TCODColor::lightRed, TCODColor::darkerRed);
	renderBar(36, 2, BAR_WIDTH, "XP", xp, 30, TCODColor::lightGreen, TCODColor::darkerGreen);
	renderBar(68, 2, BAR_WIDTH, "Books", books, books, TCODColor::amber, TCODColor::amber);

	//log rendering
	int y = 17;
	for (const auto message : log) {
		con->setDefaultForeground(message->col);
		con->print(LOG_X,y,message->text.c_str());
		y--;
		if (y == 7) {
			
			con->rect(LOG_X, y, LOG_WIDTH, LOG_HEIGHT, true);
			y = 17; 
		}
	}

	//glue console to main window
	TCODConsole::blit(con, 0, 0, root_x_max, PANEL_HEIGHT, TCODConsole::root, 0, root_y_max - PANEL_HEIGHT);
}

void Gui::renderBar(int x, int y, int width, const string name, float value, float maxValue, const TCODColor & barColor, const TCODColor & backColor) {

	con->setDefaultBackground(backColor);
	con->rect(x, y, width, 3, false, TCOD_BKGND_SET);
	int bar_level = (int)(value / maxValue * width);
	if (bar_level > 0 &&!(value>maxValue)) {
		con->setDefaultBackground(barColor);
		con->rect(x, y, bar_level, 3, false, TCOD_BKGND_SET);
	}
	//print "health" next to heath bar;
	con->setDefaultForeground(TCODColor::white);
	con->printEx(x + width / 2, y+1, TCOD_BKGND_NONE, TCOD_CENTER, "%s : %g/%g", name.c_str(), value, maxValue);
}

void Gui::log_message(const TCODColor& col, const string text, ...){

	// build the text
	va_list arguments;
	char buf[172];
	va_start(arguments, text);
	vsprintf_s(buf, text.c_str(), arguments);
	va_end(arguments);

	string temp = buf;
	string temp2;
	if (temp.size() >= LOG_WIDTH) {
		
		temp2 = temp.substr(LOG_WIDTH);
		temp = temp.substr(0, LOG_WIDTH);

		Message* msg = new Message(temp2, col);
		log.push(msg);
		Message* msg2 = new Message(temp, col);
		log.push(msg2);
	}
	else {
		Message* msg3 = new Message(buf, col);
		log.push(msg3);
	}
}

Gui::Message::Message(const string text, const TCODColor& col) :text(text),col(col) {}

Gui::Gui(int x_max, int y_max) {

	root_x_max = x_max;
	root_y_max = y_max;
	con = new TCODConsole(x_max, PANEL_HEIGHT);
}

Gui::~Gui() {

	delete con;

}
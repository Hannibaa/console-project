#include <iostream>
#include <console project/console/ka_utility.hpp>
#include <include/random/random_variable.h>



int main() {


	win::Set_ConsoleWindow_Style();
	konsole->construct_console(145, 35, 8, 16);

	RV::RVec<int> rv{ COLOR::FG_BLACK,COLOR::FG_WHITE };
	rv.set_sleepFunction([]() {std::this_thread::sleep_for(std::chrono::milliseconds(250)); });

	iRect rect(50, 10, 40, 6);
	int _color{COLOR::BG_BLUE};
	while (konsole->is_open()) {
		konsole->clear();
		iVec2 mouse = iVec2(konsole->getX(), konsole->getY());
		if (rect.contain(mouse)) {
			_color = COLOR::BG_RED;
		}
		else
		{
			_color = COLOR::BG_BLUE;
		}

		console::messageBox(100, 10, L"hello msg box", rv);

		konsole->text_at(10, 10, L"hello world", COLOR::BG_BLUE | COLOR::FG_DARK_RED);
		konsole->display();
	}

	return 0;
}
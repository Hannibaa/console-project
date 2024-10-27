#include<include/console/CConsole_utility.h>
#include<include/console/CConsole/exstring.hpp>



#define End    '\n'


int main() {

	console::ExString<float>   str2(L"hello world");

	str2 = console::ExString<float>::make(L"Hello", 10, 10, 20, COLOR::BG_BLACK, COLOR::FG_RED);

	_console.construct_console(145, 45, 7, 12);
	_console.set_background_color(COLOR::BG_BLUE);

	int i{};
	int j{};

	DWORD nEvents{ 1 };

	while (_console.is_open()) {
		_console.clear();
		++j;
		if (nEvents > 0) {
			++i;
		}

		PRINT_TITLE(L"Application iteration Events[%d] [%d] no blocking [%d]", nEvents, i, j);

		GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), &nEvents);


		console::draw(&str2);


		_console.display();
	}

	return 0;
}
#include<include/console/CConsole_utility.h>
#include<include/console/CConsole/exstring.hpp>



#define End    '\n'


int main() {

	console::ExString<float>   str2(L"hello world");

	str2 = console::ExString<float>::make(L"Hello", 10, 10, 20, COLOR::BG_BLACK, COLOR::FG_RED);

	_console.construct_console(145, 45, 7, 12);
	_console.set_background_color(COLOR::BG_BLUE);



	return 0;
}
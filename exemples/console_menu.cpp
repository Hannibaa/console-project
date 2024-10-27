#include <include/console/CConsole_utility.h>
#include <include/console/CConsole/selector.h>
#include <include/time/Game_timer.h>

#include <include/console/CConsole/menu.h>
#include <include/console/CConsole/Message_box.h>
#include <include/console/CConsole/Message_box2.hpp>
#include <include/console/CConsole/sprite.hpp>
#include <include/console/CConsole/Drag.hpp>
#include <CConsole/drawable_pack.hpp>
#include <conio.h>

using uchar = unsigned char;

int main() {

	std::wstring  string(L"Hello");
	console::ExString<int> exstring(L"Hello");
	console::CSprite_view<int>    sprite;
	sprite.set_sprite(&string, 1200, 30, 20, 10, 5, COLOR::FG_WHITE, COLOR::BG_GREEN);

	console::CDrag<int> drag(&sprite, []()->bool {return KeyPressed(VK_LBUTTON); });
	drag.set_draging_area(30, 20, 7, 3);

	// set window as unchangble
	win::Set_ConsoleWindow_Style();

	_console.construct_console(130, 35, 8, 16);
	_console.set_background_color(COLOR::BG_DARK_BLUE);

	// Menu : 
	console::CPopMenu   menu(1000, 20, COLOR::FG_WHITE, COLOR::BG_DARK_GREY, COLOR::BG_RED);

	int i{};
	menu.add_item(L"Red", []() { _console.set_background_color(COLOR::BG_RED); });
	menu.add_item(L"Black", []() {_console.set_background_color(COLOR::BG_BLACK);  });
	menu.add_item(L"Blue", []() {_console.set_background_color(COLOR::BG_DARK_BLUE); });
	//menu.add_item(L"Clear", [&]() { string.clear(); });
	//menu.add_item(L"New", [&]() {
	//	std::thread([&]() {
	//		char ch;

	//		while (true) {
	//			//ch = _getch();
	//			if (_console.keyboard_On()) {
	//				wchar_t ch = _console.get_AsciiKey();
	//				if (std::isprint(static_cast<uchar>(ch)))
	//				{
	//					string.push_back(wchar_t(ch));
	//				}
	//				else if (ch == L'\b')
	//				{
	//					if (!string.empty()) string.pop_back();
	//				}
	//				else if (ch == L'\r') {
	//					menu.Off();
	//					break;
	//				}
	//			}
	//			
	//		}
	//		}).detach(); 
	//	});

	menu.add_item(L"Close", []() {_console.close(); });



	console::CMessageBox  msg(20, 20, 30, 9, L"Titel", COLOR::BG_DARK_GREEN);
	msg.set_caption(COLOR::FG_WHITE, COLOR::BG_BLACK);

	console::CMessageBox2 msg2(20, 20, 70, 6,
		L"MSG2 - Click On Left Up Corner to drag it",
		COLOR::BG_DARK_GREEN,
		[]()->bool {return KeyPressed(VK_LBUTTON); });
	msg2.set_caption(COLOR::FG_BLACK, COLOR::BG_GREY);

	Time::Game_Timer timer;


	while (_console.is_open()) {
		// timer and frame per seconds
		console::elaps = timer.get_elapsed_time();
		console::fps = 1.f / console::elaps;
		timer.reset();

		// mous coordinate
		iVec2 mouse = iVec2(_console.get_mouseXY().X, _console.get_mouseXY().Y);

		// clear
		_console.clear();

		// title
		PRINT_TITLE(L"Application |%s| fps[%4.2f]i[%d]", string.c_str(), console::fps, i);



		menu.On_Mouse(mouse);
		menu.On_Click(mouse);

		msg.On_Select(mouse);
		msg.On_Click(mouse);
		msg.Drag(mouse);

		drag.drag(mouse);

		msg2.draging(mouse);

		console::draw(&msg2);
		console::draw(&sprite);
		console::draw(&msg);
		console::draw(&menu);
		_console.display();
	}


	return 0;
}




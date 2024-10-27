/*
				TEST OF SELECTIONABLE AND PACK 
				STILL NOT FINISH
				TODO : NOT COMPLETE
*/


#include <iostream>
#include <console project/console/ka_utility.hpp>
#include <include/time/Game_timer.h>
#include <console project/component/ka_ColorString.hpp>
#include <console project/component/tst_sprite.hpp>
#include <console project/basic/ka_Pack.h>


using Pack = console::PackDrawable<cwstring, int>;

cwstring make_text(std::wstring_view text, int x, int y, color::Color colorfg, color::Color colorbg) {

	cwstring cstr(text);
	cstr.set_position(x, y);
	cstr.color_fg(colorfg);
	cstr.color_bg(colorbg);

	return cstr;
}

template<size_t N>
void make_textes(cwstring(&arrayStr)[N], std::wstring* arraywstr);

int main() {
	color::Color bg_console = color::Blue;
	konsole->construct_console(145, 44, 7, 14);
	konsole->set_background_color(bg_console << 4);
	win::move_console_window(40, 0);
	win::Set_ConsoleWindow_Style();

	Pack pack;
	pack.set_action([]()->bool {return KeyPressed(VK_LBUTTON); });

	auto str1 = make_text(L"Hello     ", 10, 10, color::GrayDark, bg_console);
	auto str2 = make_text(L"World     ", 20, 15, color::Green, bg_console);
	auto str3 = make_text(L"Aoues     ", 10, 30, color::Magenta, bg_console);
	auto str4 = make_text(L"Kadda     ", 10, 20, color::White, color::GreenLight);

	pack.registerDrawable(&str1, {10,10, 3, 1});
	pack.registerDrawable(&str2, {20,15, 3, 1});
	pack.registerDrawable(&str3, {10,30, 3, 1});
	pack.registerDrawable(&str4, {10,20, 3, 1});

	Time::Game_Timer  timer;

	std::wstring note{L"Nothing"};

	// bool
	bool Enable_selection{ true };

	int k{};

	cwstring* prev_item{ nullptr };

	while (konsole->is_open()) {

		konsole->clear();
		KA_MAKE_FPS(timer);
		PRINT_TITLE(L"Application fps[%5.2f][%s]selection[%d]select[%d] ", ka_fps, note.c_str(),
			Enable_selection,k);

		auto mouse = iVec2{ konsole->get_mouseXY() };

		if (KeyReleased(_u('S'))) {
			Enable_selection = !Enable_selection;
		}




		pack.drag(mouse);

		pack.draw();
		konsole->display();
	}

	return 0;
}
#include <sstream>
#include <console project/console/ka_windows.hpp>
#include <console project/basic/ka_IOString.hpp>
#include <console project/console/ka_utility.hpp>
#include <console project/component/tst_sprite.hpp>
#include <include/time/Game_timer.h>


#define   USING(T)      using TRect = geom2d::Rect<T>



USING(int);


int main() {
	win::Set_ConsoleWindow_Style();
	konsole->construct_console(120, 30, 8, 16);
	konsole->set_background_color(COLOR::BG_DARK_BLUE);

	console::IOString<wchar_t>  iostring{ konsole->get_stdin() };

	// instantiate object
	//console::CSprite_view<int>   sprite;
	std::wstring wshape(L"Hello Console");
	//sprite.set_shape(&wshape);

	console::Selectionable<int>  s;
	s.set_action([]() {return KeyPressed(VK_LBUTTON); });

	Time::Game_Timer timer;

	float v{};

	int x{20}, y{10};

	std::vector<std::wstring> vecwstr;

	std::wstringstream ss;
	s.set_rectangle(x, y, 8, 1);

	bool get_key = false;
	wchar_t prev_key{};

	std::wstring _caption = L"caption";

	CONSOLE_CURSOR_INFO cursor;

	GetConsoleCursorInfo(konsole->get_stdout(), &cursor);

	cursor.bVisible = TRUE;
	cursor.dwSize = 100;

	SetConsoleCursorInfo(konsole->get_stdout(), &cursor);


	int color{};
	std::wstring str;
	while (konsole->is_open())
	{
		elaps = timer.get_elapsed_time();
		timer.reset();
		fps = 1.f / elaps;
		konsole->clear();
		PRINT_TITLE(L"Application : [%4.2f]", fps);

		auto mouse = iVec2(konsole->get_mouseXY());


		if (s.selected(mouse)) {
			color = COLOR::FG_RED;
		}
		else
			color = COLOR::FG_WHITE;

		s.dragging(mouse);
		s.drag(mouse);

		ss.str(L"");

		if (konsole->mouse_move())
			ss << L"mouse move   ";
		else
			ss << L"mouse static ";

		if (KeyReleased(VK_DELETE)) { 
			get_key = !get_key;
		}

		if (KeyPressed(_u('I')))
			if (KeyPressed(_u('J')))
			{
				iostring.read();
				iostring.clear();
				get_key = true;
			}

		if (iostring.reading())
		{
			iostring.String(str);
			_caption = L"reading...";
		}
		else {
			if (!str.empty()) {
				vecwstr.push_back(str);
				_caption = L"push new string |" + std::to_wstring(str.size());
				_caption += L" |" + std::to_wstring(vecwstr.size());
				str.clear();
			}
		}

		ss << " <";
		ss << str;
		ss << cursor.bVisible << "|" << cursor.dwSize;
		ss << "> ";
		ss << konsole->getX() << "|" << konsole->getX();
		ss << "[" << win::get_mouseCoord().x << "|" << win::get_mouseCoord().y << "]";

		console::Message_Box(s.x, s.y + 1, console::Hborder, _caption, ss.str(),
			color, COLOR::FG_GREEN, COLOR::FG_DARK_YELLOW, 
			konsole->get_background_color());


		//console::draw(&sprite);
		konsole->display();
	}



	return 0;
}
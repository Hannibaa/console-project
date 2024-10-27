#include <iostream>
#include <functional>
#include <include/console/CConsole_utility.h>
#include <include/console/CConsole/sprite.hpp>
#include <include/console/CConsole/selector.h>
#include <include/time/Game_timer.h>
#include <include/random/random_generator.h>


using Sprite    = console::CSprite_view<float>;
using SSelector = console::Selector<Sprite>;








template<typename InterfaceObject>
class Assembler : public std::vector<InterfaceObject*> {

public:
	using std::vector<InterfaceObject*>::vector;


};

Sprite  make(std::wstring* ptrwstr,int id, float x, float y, float cx, float cy,
	int fg_color, int bg_color = -1)
{
	Sprite   sprite;
	sprite.set_shape(ptrwstr);
	sprite.rectangle(x, y, cx, cy);
	sprite.set_bg_color(bg_color);
    
	sprite.set_color(fg_color);
	sprite.set_id(id);
	return sprite;
}

std::wstring make_wstring(int cx, int cy, wchar_t c) {
	std::wstring wstr(cx * cy, c);
	return wstr;
}

std::wstring make_wstring(int cx, int cy, int from)
{
	std::wstring wstr(cx * cy, 0);
	for (auto& e : wstr)
	{
		e = from++;
	}

	return wstr;
}

Sprite  make_rect(std::wstring* pwstr, int x, int y, int cx, int cy, int _color) {
	console::CSprite_view<float> sprite;

	sprite.set_shape(pwstr);

	return sprite;
}

void add_to_vec(std::vector<Sprite>& vec, int size ,std::wstring* pwstr, 
	const iVec2& xx, const iVec2& yy, const iVec2& ss ) 
{
	rng::iRG<int> irand;
	auto _size = vec.size();
	vec.reserve(_size + size + 1);

	for (int i = 0; i < size; ++i) {
		int x = irand(xx.x, xx.y);
		int y = irand(yy.x, yy.y);

		int cx = irand(ss.x, ss.y);
		int cy = irand(ss.x, ss.y);

		int fg_color = irand(1, 0x0f);
		int bg_color = irand(1, 0x0f) << 4;

		int id = irand(0, 10'000'000);

		vec.push_back(
			make(pwstr, id, float(x), float(y), float(cx), float(cy), fg_color, bg_color)
		);
	}
	
}

int main() {
	win::move_console_window(40, 40);

	_console.construct_console(145, 45, 8, 12);
	_console.set_background_color(COLOR::BG_BLUE);
	// set windows style customize unsizable, un remove close button alos
	win::Set_ConsoleWindow_Style();

	// string that represent object
	// manual adding a shapes
	std::wstring  shape(L"              ");
	std::wstring  shape2 = make_wstring(4, 3, int(L'A'));
	std::wstring  shape3 = make_wstring(5, 2, L' ');
	
	auto spr = make(&shape2,120,  50, 17, 3, 6, COLOR::FG_YELLOW, COLOR::BG_GREEN);
	auto spr1 = make(&shape,12, 100, 17, 6, 2, COLOR::FG_YELLOW, COLOR::BG_RED);
	auto spr2 = make(&shape3,222, 10, 10, 1, 14, COLOR::FG_WHITE, COLOR::BG_BLACK);
	


	// create selector
	SSelector  selector(nullptr);

	// Data
	Assembler<Sprite>  assembler { &spr, & spr1, & spr2 };
	std::vector<Sprite> vec_spr;
	add_to_vec(vec_spr, 10, &shape, { 10, 140 }, { 5, 35 }, { 1, 7 });

	for (int i = 0 ; i < vec_spr.size() ; ++i)
		assembler.push_back(&vec_spr[i]);

	bool isDraging = false;
	bool ctrl = false;

	Time::Game_Timer timer;

	int notifey_color = COLOR::BG_YELLOW;
	int select_color = COLOR::BG_WHITE;

	fVec2 pos;

	while (_console.is_open()) {
		// clear the buffer 
		_console.clear();

		// calculate frame per seconds
		console::elaps = timer.get_elapsed_time();
		timer.reset();
		console::fps = 1.f / console::elaps;

		// set application title
		swprintf_s(console::title, L"Selection Drop and Drag | fps[%5.2f] | N [%d]"
			, console::fps , assembler.size());
		TITLE(console::title);
		// set timing



		auto mouse = geom2d::fPoint2d(_console.get_mouseXY());





		for (auto& s : assembler) {
			bool is_contain = false;
			if (s->contain(mouse)) {
				is_contain = true;
				s->set_bg_color(notifey_color);
			}

			if (!ctrl) {
				if (KeyPressed(VK_LBUTTON)) {
					if (is_contain && !isDraging) {
						selector.select(s);
						isDraging = true;
						pos = s->get_position() - mouse;
					}

					if (isDraging) {
						selector.get()->set_bg_color(select_color);
						selector.get()->set_position(mouse + pos);
					}
				}
				else
				{
					isDraging = false;
				}
			}
			if (!is_contain) s->reset_bg_color();
			
		}

		if (KeyPressed(VK_CONTROL))
		{
			ctrl = true;
			if (KeyPressed(VK_LBUTTON))
			{
				selector.get()->set_bg_color(COLOR::BG_GREY);
			}
			else
			{
				selector.get()->set_bg_color(COLOR::BG_DARK_GREY);
			}
		}
		else
		{
			ctrl = false;
		}





		/*if (spr.contain(mouse))
		{
			spr.set_bg_color(COLOR::BG_DARK_GREEN);
		}
		else
		{
			spr.set_bg_color(COLOR::BG_BLACK);
		}*/

		for (const auto& s : vec_spr)
			console::draw(&s);

		console::draw(&spr2);
		console::draw(&spr1);
		console::draw(&spr);
		_console.display();
	}

	return 0;
}
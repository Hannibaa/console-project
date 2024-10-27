#include <iostream>
#include <functional>

#include <include/console/CConsole_utility.h>
#include <include/console/CConsole/sprite.hpp>
#include <include/console/CConsole/selector.h>
#include <include/time/Game_timer.h>
#include <include/random/random_generator.h>


using Sprite = console::CSprite_view<float>;
using SSelector = console::Selector<Sprite>;


class Asm : public console::IAssembler<Sprite, SSelector>
{
	int bg_color_notify{ COLOR::BG_DARK_GREEN };
	int bg_color_selec{ COLOR::BG_WHITE };

public:
	using console::IAssembler<Sprite, SSelector>::IAssembler;

	virtual void On_Mouse(Sprite* s) override {
		s->set_bg_color(bg_color_notify);
	}

	virtual void Out_Mouse(Sprite* s) override {
		s->reset_bg_color();
	}

	virtual void On_MouseDrag(Sprite* s, SSelector& select) override {
		select.get()->set_bg_color(COLOR::BG_BLACK);
	}
};






Sprite  make(std::wstring* ptrwstr, int id, float x, float y, float cx, float cy,
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

void add_to_vec(std::vector<Sprite>& vec, int size, std::wstring* pwstr,
	const iVec2& xx, const iVec2& yy, const iVec2& ss)
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
	std::wstring  shape(L"0              ");
	std::wstring  shape1 = make_wstring(4, 3, int(L'A'));
	shape1[0] = L'1'; shape1[1] = L' ';

	std::wstring  shape2 = make_wstring(5, 2, L' ');
	shape2.insert(shape2.begin(), L'2');
	std::wstring  A = L"A";

	auto spr = make(&shape, 120, 50, 17, 3, 6, COLOR::FG_YELLOW, COLOR::BG_GREEN);
	auto spr1 = make(&shape1, 12, 100, 17, 14, 3, COLOR::FG_YELLOW, COLOR::BG_RED);
	auto spr2 = make(&shape2, 222, 10, 10, 1, 10, COLOR::FG_WHITE, COLOR::BG_BLACK);
	auto spr3 = make(&A, 343, 120, 20, 3, 2, COLOR::FG_WHITE, COLOR::BG_DARK_YELLOW);


	// create selector
	SSelector  selector(nullptr);

	// Data
	Asm   assembler{ &spr, &spr1, &spr2, &spr3 };

	Time::Game_Timer timer;



	while (_console.is_open()) {
		// clear the buffer 
		auto mouse = fVec2(_console.get_mouseXY());
		_console.clear();

		// calculate frame per seconds
		console::elaps = timer.get_elapsed_time();
		timer.reset();
		console::fps = 1.f / console::elaps;

		// set application title
		swprintf_s(console::title, L"Selection Drop and Drag | fps[%5.2f] | N [%d]"
			, console::fps, assembler.size());
		TITLE(console::title);


		assembler.Selection(mouse, selector);


		console::draw(&assembler);
		_console.display();
	}

	return 0;
}
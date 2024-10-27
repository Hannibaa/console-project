#include <iostream>
#include <include/console/CConsole_utility.h>
#include <include/time/Game_timer.h>
#include <include/console/CConsole/sprite.hpp>
#include <include/console/CConsole/gcstring.hpp>


std::pair<COORD, COORD>  get_console_window_rect() {
	HWND hwind = GetConsoleWindow();

	RECT rect{};
	GetWindowRect(hwind, &rect);

	COORD coord = { rect.left, rect.top };
	COORD sizexy = { rect.right - rect.left, rect.bottom - rect.top };

	return std::pair(coord, sizexy);
}


//class Animation {
//
//	_box* pbox;
//
//	float _vx, _vy;
//	float m_lx, m_ly;
//
//public:
//	Animation(_box* _pbox, float vx, float vy)
//		:pbox(_pbox),
//		_vx(vx),
//		_vy(vy),
//		m_lx(_console.get_size().first),
//		m_ly(_console.get_size().second)
//	{}
//
//	void Move(float dt) {
//		float x = pbox->getX();
//		float y = pbox->getY();
//
//		if (KeyPressed(VK_UP)) {
//			y -= _vy * dt;
//		}
//		if (KeyPressed(VK_DOWN)) {
//			y += _vy * dt;
//		}
//		if (KeyPressed(VK_LEFT)) {
//			x -= _vx * dt;
//		}
//		if (KeyPressed(VK_RIGHT)) {
//			x += _vx * dt;
//		}
//
//		float Lx = pbox->get_boundsX();
//		float Ly = pbox->get_boundsY();
//
//		x = std::clamp(x, 0.f, m_lx - Lx);
//		y = std::clamp(y, 0.f, m_ly - Ly - 1);
//
//		pbox->set_position(x, y);
//	}
//};
//
//
//class Transform {
//
//	_box* _pbox;
//	std::vector<std::wstring>   _shapes;
//	int							_nshapes;
//	int							i;
//
//public:
//	template<typename... STRING>
//	Transform(const STRING&...wstr) : i(0) {
//		(_shapes.push_back(wstr), ...);
//		_nshapes = _shapes.size();
//	}
//
//	void set_box(_box* pbox) {
//		_pbox = pbox;
//		_shapes.push_back(_pbox->get_shape());
//	}
//
//	void transform() {
//
//		if (KeyReleased(_u('A'))) { // move between strings
//			++i;
//		}
//		if (KeyReleased(_u('F'))) { // return to original 
//			--i;
//		}
//		i = std::clamp(i, 0, _nshapes);
//		_pbox->set_shape(_shapes[i].c_str());
//
//	}
//};

// mouse selection option and drop : any object : 



#define      End    '\n'


int main() {
	_console.construct_console(145, 35, 8, 14);
	_console.set_background_color(COLOR::BG_BLUE);
	Time::Game_Timer   timer;

	// get initial coordinate of console
	auto czwin_init = get_console_window_rect();

	auto win_coord = czwin_init.first;

	console::gcstring  gstr(L"hello color", std::vector<int>{COLOR::BG_DARK_BLUE, 2,
														4, 5, 6, 7, 8, 3, 9, 10, 0x001c, 0x0701});
	gstr.set_position(120.f, 20.f);

	console::CSprite_view<float>    sprite;
	sprite.rectangle(70.f, 20.f, 6.f, 10.f);

	std::wstring  w(L"         XX             ");
	sprite.set_shape(&w);
	sprite.set_id(1000);
	sprite.set_color( COLOR::FG_RED);
	//sprite.set_bg_color(COLOR::BG_DARK_BLUE);
	sprite.set_position(50.f, 15.f);

	std::vector<int> vc;
	vc.reserve(17);
	COORD* ptr_cord = nullptr;
	fVec2* ptr_point = nullptr;

	COORD  cst_coord = { 50, 50 };
	ptr_cord = &cst_coord;

	auto pos_sprite = sprite.get_position();

	auto pfont = _console.get_font_size();
	int Lx = pos_sprite.x + win_coord.X/pfont.x;
	int Ly = pos_sprite.y + win_coord.Y/pfont.y;
	
	ptr_point = &pos_sprite;
	fVec2  mouse;

	// Create loop for game
	while (_console.is_open()) {
		// set timer
		timer.reset();
		// clear screen 
		_console.clear();
		// set title
		auto coord = _console.get_mouseXY();
		mouse.x = coord.X; mouse.y = coord.Y;

		auto spr_coord = sprite.get_position();

		swprintf_s(console::title, 250, L"fps|DT[%4.2f|%2.6f]xy[%3.2f|%3.2f]",
			console::fps, console::Dtime, spr_coord.x, spr_coord.y);// float(coord.X), (float)coord.Y);
		TITLE(console::title);

		for (int i = 0; i < 11; ++i) vc.push_back((rand() % 240));

		gstr.set_colors(vc);
		vc.clear();

		// get moved windows coordinate :
		win_coord = get_console_window_rect().first;
		pos_sprite.x = Lx - float(win_coord.X)/pfont.x ;
		pos_sprite.y = Ly - float(win_coord.Y)/pfont.y ;
		
		sprite.set_position(ptr_point->x , ptr_point->y);

		if (KeyReleased(VK_RBUTTON)) {
			ptr_cord = &coord;
		}

		if (KeyReleased(VK_LBUTTON)) {
			cst_coord = coord;
			ptr_cord = &cst_coord;
		}

		if (sprite.contain(mouse)) {
			int array_color[2] = {COLOR::BG_DARK_GREEN, COLOR::BG_DARK_RED};
			sprite.set_bg_color(array_color[rand()%2]);
		}

		if (KeyPressed(VK_RBUTTON)) {
			ptr_point = &mouse;
			pos_sprite = mouse;
			// calculate new L values :
			Lx = pos_sprite.x + win_coord.X / pfont.x;
			Ly = pos_sprite.y + win_coord.Y / pfont.y;
		}
		else
			ptr_point = &pos_sprite;

		gstr.set_position(ptr_cord->X, ptr_cord->Y);
		console::draw(&gstr);
		console::draw(&sprite);

		_console.display();
		console::elaps = timer.get_elapsed_time();
		console::fps = 1.f / console::elaps;
		console::Dtime = console::Dt60 * 60.f / console::fps;
	}

	return 0;
}
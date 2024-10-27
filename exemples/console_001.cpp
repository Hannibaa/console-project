#include <iostream>
#include <include/console/CConsole_utility.h>
#include <include/time/Game_timer.h>


class _box {

	std::wstring    _shape;

	int              _size;

	float           _x, _y;
	int 		  _lx, _ly;

	int				_color;
	int				_bg_color;

	void check() {

		if (_lx > _size) {
			_lx = _size;
			_ly = 0;
			return;
		}

		int i = _size - 1;
		_ly = (i - i % _lx) / _lx;
	}

public:
	_box(const std::wstring& shape, int lx, int ly)
		:_shape(shape),
		_size(shape.size()),
		_lx(lx),
		_ly(ly),
		_bg_color(_console.get_background_color())
	{
		check();
	}

	void set_shape(const wchar_t* new_shape) {
		_shape.clear();
		_shape = std::wstring(new_shape);
		_size = _shape.size();
		check();
	}

	std::wstring get_shape() const {
		return _shape;
	}

	void clear() {
		_shape.clear();
	}

	void set_color(int color) {
		_color = _bg_color | color;
	}

	float getX() const {
		return _x;
	}

	float getY() const {
		return _y;
	}

	float get_boundsX() const {
		return _lx;
	}

	float get_boundsY() const {
		return _ly;
	}

	void set_position(float x, float y) {
		_x = x;
		_y = y;
	}

	void draw() const {
		for (int i = 0; i < _size; ++i)
		{
			int k = i % _lx;
			int x = int(_x) + k;
			int y = int(_y) + (i - k) / _lx;
			_console.draw(x, y, _shape[i], _color);
		}
	}

};


class Animation {

	_box* pbox;

	float _vx, _vy;
	float m_lx, m_ly;

public:
	Animation(_box* _pbox, float vx, float vy)
		:pbox(_pbox),
		_vx(vx),
		_vy(vy),
		m_lx(_console.get_size().first),
		m_ly(_console.get_size().second)
	{}

	void Move(float dt) {
		float x = pbox->getX();
		float y = pbox->getY();

		if (KeyPressed(VK_UP)) {
			y -= _vy * dt;
		}
		if (KeyPressed(VK_DOWN)) {
			y += _vy * dt;
		}
		if (KeyPressed(VK_LEFT)) {
			x -= _vx * dt;
		}
		if (KeyPressed(VK_RIGHT)) {
			x += _vx * dt;
		}

		float Lx = pbox->get_boundsX();
		float Ly = pbox->get_boundsY();

		x = std::clamp(x, 0.f, m_lx - Lx);
		y = std::clamp(y, 0.f, m_ly - Ly - 1);

		pbox->set_position(x, y);
	}
};


class Transform {

	_box* _pbox;
	std::vector<std::wstring>   _shapes;
	int							_nshapes;
	int							i;

public:
	template<typename... STRING>
	Transform(const STRING&...wstr) : i(0) {
		(_shapes.push_back(wstr), ...);
		_nshapes = _shapes.size();
	}

	void set_box(_box* pbox) {
		_pbox = pbox;
		_shapes.push_back(_pbox->get_shape());
	}

	void transform() {

		if (KeyReleased(_u('A'))) { // move between strings
			++i;
		}
		if (KeyReleased(_u('F'))) { // return to original 
			--i;
		}
		i = std::clamp(i, 0, _nshapes);
		_pbox->set_shape(_shapes[i].c_str());

	}
};




int main() {

	// construct console as buffer
	_console.construct_console(145, 45, 8, 12);
	_console.set_background_color(COLOR::BG_BLUE);
	Time::Game_Timer timer;


	_box box(L"hello box first time", 6, 5);
	box.set_color(COLOR::FG_WHITE);
	box.set_position(50, 10);

	Animation anim(&box, 1.f, 1.f);
	Transform tr(L"This my names", L"here it is", L"that !");
	tr.set_box(&box);

	while (true) {
		// Break loop
		timer.reset();
		if (KeyOn(VK_ESCAPE)) break;

		// set background blue;
		_console.clear();

		swprintf(console::title, 250, L"Windows[%3d, 3%d] | fps[%4.2f] |[%f,%f]DT[%f]",
			_console.get_size().first,
			_console.get_size().second,
			console::fps, box.getX(), box.getY(), console::Dtime);

		TITLE(console::title);





		tr.transform();
		anim.Move(console::Dtime);
		box.draw();

		_console.text_at(70, 20, L"Hello Console", COLOR::BG_BLACK | COLOR::FG_DARK_RED);

		_console.display();

		console::elaps = timer.get_elapsed_time();
		// set fps
		console::fps = 1.f / console::elaps;
		console::Dtime = console::Dt60 * 60.f / console::fps;
	}




	return 0;

}
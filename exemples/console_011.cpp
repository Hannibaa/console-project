#include <console project/console/ka_utility.hpp>
#include <console project/component/ka_ColorString.hpp>
#include <console project/component/ka_ICinematics.hpp>
#include <console project/component/ka_ISprite.hpp>
#include <include/time/Game_timer.h>
#include <include/maths/clamp.hpp>



class Shipe : public cwstring {
	// definition of motion
	cinematic::IMove*			_motion;

public:
	Shipe(const cwstring& _str) : cwstring(_str) {}

	void set_motion(cinematic::IMove* motion) {
		_motion = motion;
	}

	void move(float Dt) {
		fVec2 p = this->get_position();
		fVec2 dim = this->get_dimension();
		if (_motion)
		_motion->move(p, Dt, dim);
		this->set_position(p);
	}

};

// free motion
class FreeMotion : public cinematic::IMove {
	float v1{ 1.0f };
	float v2{ 1.0f };

public:

	void move(fVec2& p,float Dt, const fVec2& dim) override {

		p.x += _speed.x * Dt * v1;
		p.y += _speed.y * Dt * v2;

		if (p.x < _field.x || p.x > _field.x + _field.dx - dim.x + 2.f) v1 *= -1.f;
		if (p.y < _field.y || p.y > _field.y + _field.dy - dim.y + 2.f) v2 *= -1.f;
	}
};
// controlled motion
class ControlMotion : public cinematic::IMove {
public:

	void move(fVec2& p, float Dt, const fVec2& dim) override {

		if (KeyPressed(VK_UP)) {
			p.y -= _speed.y * Dt;
		}

		if (KeyPressed(VK_DOWN)) {
			p.y += _speed.y * Dt;
		}
		
		if (KeyPressed(VK_LEFT)) {
			p.x -= _speed.x * Dt;
		}
		
		if (KeyPressed(VK_RIGHT)) {
			p.x += _speed.x * Dt;
		}

		math::clamp<float>(p.x, 0, 145 - dim.x);
		math::clamp<float>(p.y, 0, 35 - dim.y);
	}
};
// gravitation motion in ground56
class JumpMotion : public cinematic::IMove {
	float			_ground{ 35.f };
	float			_intensity{ 1.23f };
public:

	void set_ground(float ground) {
		_ground = ground;
	}

	void set_intensity(float i) {
		_intensity = i;
	}

	void move(fVec2& p, float Dt, const fVec2& dim) override {

		if (KeyPressed(VK_UP)) {
			p.y -= _intensity * _speed.x * Dt;
		}

		if (KeyPressed(VK_LEFT)) {
			p.x -= _speed.x * Dt;
		}

		if (KeyPressed(VK_RIGHT)) {
			p.x += _speed.x * Dt;
		}

		p.y += _speed.x * Dt;

		math::clamp<float>(p.y, 0, _ground - dim.y);
		math::clamp<float>(p.x, 0, 145 - dim.x);
	}
};

class oJumpMotion : public cinematic::IMove {
	float			_intensity{ 2.f };
	float			_ground{ 25.f };

public:
	template<typename TTransformable>
	void operator()(TTransformable* ptr_, float Dt) {

		if (ptr_ == nullptr) return;

		fVec2 p = ptr_->get_position();
		fVec2 dim = ptr_->get_dimension();

		if (KeyPressed(VK_UP)) {
			p.y -= _intensity * _speed.x * Dt;
		}

		if (KeyPressed(VK_LEFT)) {
			p.x -= _speed.x * Dt;
		}

		if (KeyPressed(VK_RIGHT)) {
			p.x += _speed.x * Dt;
		}

		p.y += _speed.x * Dt;

		math::clamp<float>(p.y, 0, _ground );
		math::clamp<float>(p.x, 0, 145 - dim.x);

		ptr_->set_position(p);
	}

	void move(fVec2&, float, const fVec2&) override {
		return;
	}
};


int main() {
	const int cwidth = 145;
	const int cheight = 35;
	const int cwfont = 8;
	const int chfont = 14;

	konsole->construct_console(cwidth, cheight, cwfont, chfont);
	konsole->set_background_color(color::Blue << 4);

	cwstring  str(std::wstring(10, L'*'), color::Red, color::Blue << 4);
	str.set_position(1.f, 1.f);
	str.color_bg(color::Blue);

	auto wbox = console::make_box(L'*', 5, 6);

	Shipe shipe(str);

	// create motion
	JumpMotion jmotion;
	jmotion.set_speed(1.f, 1.f);
	jmotion.set_rectangle({ 0,0,145,35 });

	oJumpMotion jump;
	jump.set_speed(1.f, 1.f);
	jump.set_rectangle({ 0,0,145,35 });
	

	ControlMotion cmotion;
	cmotion.set_speed(1.f, 1.f);
	cmotion.set_rectangle({ 0,0,145,35 });

	FreeMotion fmotion;
	fmotion.set_speed(0.5f, 0.5f);
	fmotion.set_rectangle({ 0,0,145,35 });

	// create sprite to be moved
	wsprite     tank(L"/--\\==****  ****  ", 6, 2);
	tank.color_bg(color::Blue);
	tank.color_fg(color::White);

	wsprite		sprite(wbox._sprite, wbox._lx, wbox._ly);
	sprite.color_fg(color::White);
	sprite.color_bg(color::Blue);

	shipe.set_motion(&jmotion);

	Time::Game_Timer  timer;
	int count{};
	iVec2 mouse;

	while (konsole->is_open()) {
		konsole->clear();

		KA_MAKE_FPS(timer);
		KA_MAKE_DALTA_TIME(1);

		while (konsole->pull_event()) {
			++count;
			mouse = iVec2(konsole->get_mouseXY());
		}

		if (KeyReleased(_u('C'))) {
			shipe.set_motion(&cmotion);
		}

		if (KeyReleased(_u('J'))) {
			shipe.set_motion(&jmotion);
		}

		if (KeyReleased(_u('F'))) {
			shipe.set_motion(&fmotion);
		}


		PRINT_TITLE(L"Application fps[%5.2f] Dt[%f]Mouse[%d][%d][%d]", ka_fps, ka_Dt,
			mouse.x, mouse.y , count);

		shipe.move(ka_Dt);
		jump(&tank, ka_Dt);

		console::draw(&tank);
		console::draw(&shipe);
		console::draw(&sprite);
		konsole->display();
	}




	return 0;
}
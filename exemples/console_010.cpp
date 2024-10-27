#include <console project/console/ka_utility.hpp>
#include <console project/component/ka_ColorString.hpp>
#include <include/time/Game_timer.h>
#include <include/maths/clamp.hpp>
#include <console project/component/Cinematics.hpp>



int main() {

	const int Width = 145;
	const int Height = 35;

	konsole->construct_console(Width, Height, 8, 14);
	konsole->set_background_color(COLOR::BG_DARK_BLUE);
	win::Set_ConsoleWindow_Style();

	Time::Game_Timer  timer;
	std::wstring text(L" hello worlds ; this my first window text");

	int x{}, y{};

	cinematic::Motion   motion;
	motion.set_bounds(0, 0, Width, Height);
	motion.set_speed(0.5f, 0.5f);

	cinematic::FreeMotion  freemotion;
	freemotion.set_bounds(0, 0, Width - 50, Height- 10);
	freemotion.set_speed(3.0f, 3.0f);

	console::Transformable<float> anyObj; 
	anyObj.set_position(10.f, 10.f);

	auto option = console::BuildMessage()
		.set_title(L"Windows")
		.set_body(L"We Are Builder Design Pattern")
		.set_border(console::Lborder)
		.set_body_color(color::White)
		.set_title_color(color::Red)
		.set_border_color(color::Red)
		.set_background_color(color::Blue << 4)
		.get();

	while (konsole->is_open()) {
		konsole->clear();

		KA_MAKE_FPS(timer);
		KA_MAKE_DALTA_TIME(0.1f);

		PRINT_TITLE(L"Application fps[%5.2f] Dt [%f]", ka_fps, ka_Dt);

		auto m = konsole->get_mouseXY();

		if (KeyReleased(VK_UP)) {
			--y;
		}

		if (KeyReleased(VK_DOWN)) {
			++y;
		}

		freemotion(&anyObj, ka_Dt);

		fVec2 pp = freemotion.get();


		
		math::clamp(y, 0, Height - 1);
		console::box(0, y, Width, 1, color::Yellow, color::White << 4);

		console::Message_Box(pp.x, pp.y, console::Hborder, L"Windows", text,
			color::Yellow, color::White, color::Red, color::Blue << 4);

		option.x = m.X;
		option.y = m.Y;
		console::Message_Box(option);

		

		konsole->display();
	}






	return 0;
}
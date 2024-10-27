#include <console project/console/ka_utility.hpp>
#include <include/time/Game_timer.h>
#include <console project/component/ka_ColorString.hpp>
#include <include/variational_var.h>
#include <console project/basic/ka_Pack.h>
#include <console project/basic/ka_ISelectionable.hpp>




int main() {

	konsole->construct_console(145, 35, 8, 14);
	konsole->set_background_color(COLOR::BG_DARK_BLUE);



	cwstring  str(std::wstring(10,L' '), color::Red, color::White << 4);

	var::Var0<float>   x(10.f,100.f,0.01f);
	var::Var0<float>   y(30.f, 31.f, 0.01f);

	var::_Var_<float>  xx(10.f, 0.f, 140.f, 0.01f);
	var::_Var_<float>  yy(10.f, 0.f, 30.f, 0.01f);

	bool select_string = false;

	Time::Game_Timer timer;
	while (konsole->is_open())
	{
		konsole->clear();
		KA_MAKE_FPS(timer);
		PRINT_TITLE(L"Application fps[%5.2f]str[%3d][%3d]", 
			ka_fps, str.get_position().x, str.get_position().y);

		auto mouse = iVec2(konsole->getX(), konsole->getY());


		if (KeyReleased(_u('S'))) select_string = !select_string;

		//if (select_string)
		//str.set_position(mouse);

		console::box(mouse.x, mouse.y, 10, 10, COLOR::FG_WHITE, COLOR::BG_BLACK);

		if (select_string) {
			x.target(mouse.x);
			y.target(mouse.y);
		}

		str.set_position(xx, yy);

		console::draw(&str);
		konsole->display();
	}



	return 0;
}
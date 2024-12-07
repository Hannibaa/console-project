#pragma once
#include <console project/console/ka_utility.hpp>

#include <include/geometry/rectangle.hpp>
#include <console project/component/Physics/ka_Kinematics.h>
#include <console project/component/ka_SString.hpp>
#include <console project/component/ka_ImageString.hpp>
#include <console project/component/ka_Draw.hpp>
#include <console project/component/TextUI/ka_StatuBar.hpp>
#include <include/random/random_generator.h>
#include <console project/component/Physics/ka_Physics.hpp>

#define _tow(token)				std::to_wstring(token)
#define end_					'\n'


using std::cout;
using std::cin;
const float PI = 3.1415f;

static void box(iVec2 p1, iVec2 p2) {
	int x{}, y{}, dx{}, dy{};

	x = p1.x - p2.x;
	y = p1.y - p2.y;



	Draw::border_box(console::Hborder, x, y, dx, dy,
		color::Red, color::BlueLight);

}





int Game( int W, int H, int fw, int fh) {

	const int margin = 2;
	
	bool b_squeeze{ false };
	bool b_collision{ false };
	std::wstring collision_stat[2]{ L"Enabl", L"Desable" };
	int iStat{ 0 };

	iVec2 p1{};
	iVec2 p2{};
	bool b_On{ true };


	fVec2 v0{ 0.2f, 0.1f };
	fRect window{ 0.f,0.f, float(W), float(H - margin) };

	// construction of console 
	konsole->construct_console(W, H, fw, fh);
	// set background color
	konsole->set_background_color(COLOR::BG_BLUE);
	// set drawable windows
	konsole->set_drawWindows(0, 0, W, H - margin);

	///////////////////////////////////// -- Data -- ///////////////////////

	std::vector<WSString>  vStr;

	wstringImg  box;

	for (auto i = 0; i < 16; ++i)
	{
		WSString str;
		str.append(L"HELLO " + _tow(i));
		str.set_colorBG(color::Yellow);
		str.set_colorFG(color::Black);
		str.set_position(0.f, i);
		vStr.push_back(str);
	}

	std::vector<physic::FreeMove1> vMotion;
	rng::fRG<float>  frand;

	for (auto i = 0; i < 16; ++i)
	{
		physic::FreeMove1 motion;
		motion.set_velocity(frand(-0.2f, 0.3f) , frand(-0.2f, 0.2f));
		motion.set_space(window);
		vMotion.push_back(motion);
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




	// --------------------------------------------------------------------

	physic::FreeMove1 freemove1;
	freemove1.set_velocity(v0);
	freemove1.set_space(window);


	physic::FreeMove2<WSString>  freemove2;
	freemove2.set_velocity(v0);
	freemove2.set_space(window);


	physic::FreeMove0<WSString>  freemove0;
	freemove0.set_velocity(v0);
	freemove0.set_space(window);


	Time::Game_Timer timer;
	fVec2 mouse{};


	while (konsole->is_open()) {

		konsole->clear();

		KA_MAKE_FPS(timer);
		KA_MAKE_DALTA_TIME(1.0);
		//if (konsole->pull_event()) {
		//	mouse = konsole->get_mouseXY();
		//}

		while (konsole->pull_event());
		mouse = fVec2( konsole->get_mouseXY() );


		PRINT_TITLE(L"Application fps[%4.2f]", ka_fps );

		if (KeyPressed(VK_SPACE)) {
			b_squeeze = ! b_squeeze;
		}

		if (KeyReleased(_u('A'))) {
			b_collision = ! b_collision;
		}

		if (b_squeeze) {
			goto _End;
		}

		for (int i = 0; i < 16; ++i) {
			vMotion[i].move(&vStr[i], ka_Dt);
			auto p = vStr[i].get_position().as<int>();
			vStr[i].assign(L"M- " + _tow(p.x) + L"|" + _tow(p.y));
		}

		if (b_collision) {
			physic::collision_process(vStr);
		}

		// selection methode

		if (KeyPressed(VK_LBUTTON)) {
			if (b_On) {
				p1 = mouse.as<int>();
				b_On = false;
			}

			p2 = mouse.as<int>();
		}
		else
			b_On = true;

		//if (p1.x > p2.x) std::swap(p1.x, p2.x);
		if (p1.y > p2.y) std::swap(p1.y, p2.y);

		//if (!b_On)
		//Draw::border_box(console::Hborder, p1.x, p1.y, std::abs(p2.x - p1.x),
		//	std::abs(p2.y - p1.y),
		//	color::Red, color::BlueLight);

		Draw::border_box(console::Hborder, p2.x, p2.y, std::abs(p2.x - p1.x),
			std::abs(p2.y - p1.y),
			color::Red, color::BlueLight);



	_End:
		TextUI::StatuBar(TextUI::StatuBarBuilder{}
			.set_step(1)
			.set_colorBG(color::Yellow << 4)
			.set_colorFG(color::White)
			.set_colorChar(color::Red)
			.set_position(1, H - margin + 1)
			.add_item(L"Space Squeeze")
			.add_item(L"A Collision " + collision_stat[int(!b_collision)])
			.add_item(L"Selection[" + _tow(p1.x) + L"|" + _tow(p1.y) + L"]" + 
				L"[" + _tow(p2.x) + L"|" + _tow(p2.y) + L"]")
			.add_item(L"Mouse : " + _tow(int(mouse.x)) + L"|" + _tow(int(mouse.y)))
			.get());


		for (const auto& s : vStr) s.draw();
		box.draw();
		konsole->display();
	}

	return 1;
}
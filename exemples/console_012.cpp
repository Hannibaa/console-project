#include <console project/component/utils.hpp>



int main() {
	const int cwidth = 145;
	const int cheight = 35;
	const int cwfont = 8;
	const int chfont = 14;

	konsole->construct_console(cwidth, cheight, cwfont, chfont);
	konsole->set_background_color(color::Blue << 4);

	//////////////////////////////////////// PACK MANAGER ///////////////////

	utils::Pack2<wsprite>  pack;

	utils::Pack2<cinematic::IMotion*>  packMotion;


	using pwsprite = std::unique_ptr<wsprite>;

	auto MakeSpriteBox = [](int min_l, int max_l)-> wsprite {

		RNG::iRG<int> irand;
		RNG::RG<int> x(0, 140);
		RNG::RG<int> y(0, 35);

		int l = irand(min_l, max_l);
		int w = irand(min_l, max_l);

		auto ibox = console::make_box(console::Hborder, l, w);

		wsprite     box(ibox);

		box.color_bg(color::Blue);
		box.color_fg(color::Cyan);
		box.set_position(x(), y());

		return box;
		};


	////////////////////////////////////////  DATA  /////////////////////////
	wsprite* ptr{ nullptr };
	wsprite* ptr2{ nullptr };

	cinematic::IMotion* pmotion{ nullptr };

	cwstring  str(std::wstring(10, L'*'), color::Red, color::Blue << 4);
	str.set_position(1.f, 1.f);
	str.color_bg(color::Blue);

	auto wbox = console::make_box(L'*', 5, 6);
	auto w2box = console::make_box(console::Hborder, 4, 4);
	auto w3box = console::make_box(console::Hborder, 10, 10);

	std::wstring space = L"\\|/"
						 L"-*-"
						 L"/|\\";
	
	wsprite space_shipe(space, 3, 3);
	space_shipe.color_fg(color::Red);
	space_shipe.color_bg(color::Blue);
	space_shipe.set_position(60, 10);


	wsprite     tank(L"/--\\==****  ****  ", 6, 2);
	tank.color_bg(color::Blue);
	tank.color_fg(color::White);

	wsprite		sprite(wbox);
	sprite.color_fg(color::White);
	sprite.color_bg(color::Blue);

	wsprite     box(w3box);
	box.color_bg(color::Blue);
	box.color_fg(color::Cyan);
	box.set_position(50, 10);

	wsprite     box2(w2box);
	box2.color_bg(color::Blue);
	box2.color_fg(color::Cyan);
	box2.set_position(10, 10);

	utils::Sstring  str2(L"hello", color::White, color::Red);
	str2.set_position({30.f, 1.f});

	pack.add(tank);
	pack.add(sprite);
	pack.add(box);
	pack.add(box2);
	pack.add(space_shipe);

	////////////////////////////////  MOTION  ///////////////////////////////
	utils::Jump jump;
	jump.set_speed(1.1f);
	jump.set_rectangle(0, 0, cwidth, cheight);

	utils::Control control;
	control.set_speed(1.1f);
	control.set_rectangle(0, 0, cwidth, cheight);

	cinematic::FreeMotion   freemotion(
		[](float x)->float { return x; },
		[](float y)->float { return y; },
		2.f
	);

	freemotion.set_speed(3.f);
	freemotion.set_rectangle(0, 0, cwidth, cheight);

	cinematic::FreeBoxMotion boxmotion;

	boxmotion.set_speed(1.3f);
	boxmotion.set_rectangle(0, 0, cwidth, cheight);

	packMotion.add(&jump);
	packMotion.add(&control);
	packMotion.add(&freemotion);


	ptr = &pack.at(0);
	pmotion = &jump;

	Time::Game_Timer  timer;
	int count{};
	fVec2 mouse;
	std::wstring stat;

	int index{};
	int cindex{};
	bool is_mouse_move = false;


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//			MAIN LOOP    
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

	while (konsole->is_open()) {
		konsole->clear();

		KA_MAKE_FPS(timer);
		KA_MAKE_DALTA_TIME(1);

		while (konsole->pull_event()) {
			++count;
			mouse = fVec2(konsole->get_mouseXY());
			is_mouse_move = konsole->mouse_move();
		}

		PRINT_TITLE(L"Application fps[%5.2f] Dt[%f]Mouse[%f][%f][%d]pack Size[%d]",
			ka_fps, ka_Dt,
			mouse.x, mouse.y, count, pack.size());

		if (KeyReleased(_u('A'))) {
			auto s = MakeSpriteBox(2,10);
			pack.add(std::move(s));
		}

		if (KeyReleased(_u('S'))) {
			// this very dangerous if vector inside pack resized
			ptr = &pack.at((++index) % pack.size());
		}


		if (KeyReleased(_u('J'))) {
			pmotion = &jump;
		}

		if (KeyReleased(_u('C'))) {
			pmotion = &control;
		}

		if (KeyReleased(_u('F'))) {
			pmotion = &freemotion;
			freemotion.initial_point(1.f);
		}

		if (KeyReleased(_u('B'))) {
			pmotion = &boxmotion;
		}

		fVec2 temp_mouse{};
		if (KeyPressed(VK_LBUTTON)) {
				ptr2 = pack.select_item(mouse);
		}
		else
		{
			ptr2 = nullptr;
		}

		if (is_mouse_move) {
			if (ptr2) {
				fVec2 pp = ptr2->get_dimension();
				ptr2->set_position(mouse - pp / 2.0f);
			}
		}

		// chosing object->movement .
		(*pmotion)(ptr, ka_Dt);

		pack.manage_collision();

		console::draw(&str2);
		pack.draw();
		konsole->display();
	}




	return 0;
}
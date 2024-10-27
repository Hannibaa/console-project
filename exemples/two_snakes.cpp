#include <console project/component/utils.hpp>
#include <console project/component/TextUI/ka_StatuBar.hpp>
#include <console project/component/TextUI/ka_MsgBox.hpp>
#include <console project/component/SnakeGame/snake.hpp>
#include <include/random/random_variable.h>
#include <console project/basic/ka_types.hpp>

#define _tow    std::to_wstring
#define _S(str)			#str
#define _L(str)			L#str
#define __L(str)		_L(str)

namespace snake {

	template<typename ...Ts>
	wstringImg make_One(const Ts&...) {

		RV::RVec<int>   col{ color::Red, color::Green, color::Black , color::Yellow, color::Magenta };

		return strImg::make_StringImage(2, 2, col());
	}

	template<>
	wstringImg make_One<int , int>(const int& lx, const int& ly) {

		RV::RVec<int>   col{ color::Red, color::Green, color::Black , color::Yellow, color::Magenta };
		return strImg::make_StringImage(lx, ly, col());
	}

	template<>
	std::vector<wstringImg> make_Multi<size_t>(const size_t size) {
		std::vector<wstringImg> vstr(size);

		for (auto& e : vstr) e = make_One(2,2);

		return vstr;
	}
}

static void register_name(WString& name1, WString& name2)
{
	std::cout << "Snake Game Version 1.0.0\n";
	std::cout << "Kadda Aoues Software Design\n";
	std::cout << "Two Player : " << end_;
	std::cout << "Name 1 : ";
	std::wcin >> name1;
	std::cout << "Name 2 : ";
	std::wcin >> name2;
}

template<typename Xrandom, typename IntRandom, typename Stratigy>
void collision(snake::Snake& _snake, wstringImg& food, wstringImg& poison,Stratigy& stratigy, 
	Xrandom& x, Xrandom& y , IntRandom& col) {
	if (physic::is_collid(_snake, food)) {
		_snake.add(food);
		stratigy(food, poison, x(), y(), col());
	}

	if (physic::is_collid(_snake, poison)) {
		if (_snake.get_size() > 1) {
			_snake.pop();
			poison.set_position(x(), y());
		}
	}
}


void collision(snake::Snake& _snake, std::vector<wstringImg>& vfoods) {
	for (auto it = vfoods.begin(); it != vfoods.end(); ) {
		if (physic::is_collid(_snake, *it)) {
			_snake.add(*it);
			it = vfoods.erase(it);
		}
		else
			++it;
	}

}

int main() {
	
	std::wstring name1{_L(adem)};
	std::wstring name2{_L(karim)};

	//register_name(name1, name2);


	const int cwidth		= 145;
	const int cheight		= 35;
	const int cwfont		= 8;
	const int chfont		= 14;
	const int index			= 2;
	const float Velocity	= 0.6f;

	// construct window console
	konsole->construct_console(cwidth, cheight, cwfont, chfont);
	// set drawable window
	konsole->set_drawWindows(0.f, 0.f, cwidth, cheight - index);
	// set background color
	konsole->set_background_color(color::Blue << 4);

	fRect window(0.f, 0.f, cwidth, cheight - index);



	/////////////////////////////// Designe Snake /////////

	constexpr size_t Nelements = 10;
	fVec2 p0{ 10.f, 10.f };
	auto snake_elements = snake::make_snake<Nelements>(2, 2);
	snake::StaticSnake<Nelements> snake(snake_elements, p0);


	// andd new snake
	snake::Snake    _snake(p0);
	snake::Snake	_snake2({ 120.f, 10.f });

	snake::Snake::value_type food = snake::make_One();
	snake::Snake::value_type poison = snake::make_One();


	auto stratigy_1 = [](wstringImg& food, wstringImg& poison, float x, float y, int col) {
		food.set_position(x, y);
		food.color_bg(col);
		rng::iRG<int> d;
		poison.set_position(x + d(-2, 2), y + d(-2, 2));
		};

	rng::RG<int>  x(2, cwidth - 3);
	rng::RG<int>  y(2, cheight - 3);
	RV::RVec<int> c{ color::Red,color::Yellow, color::Green, color::Yellow };

	food.set_position(x(), y());
	food.color_bg(c());

	poison.set_position(x(), y());
	poison.color_bg(color::GrayDark);

	//////////////////////////////// Create Motion ////////

	cinematic::IMotion* ptr_motion{ nullptr };


	////////////////////////////////// Controled motion
	fVec2 v0{ 0.32f, 0.22f };
	snake::control::SnakeFreeMotion freeMotion(2.f, 140.f, 2.f, 30.f, 3);
	freeMotion.set_rectangle(window);
	freeMotion.set_speed(v0);

	snake::control::SnakeControl scontrol;
	scontrol.set_rectangle(window);
	scontrol.set_speed(v0);

	snake::control::SnakeControl scontrol2;
	scontrol2.set_control('A', 'S', 'D', 'W');
	scontrol2.set_rectangle(window);
	scontrol2.set_speed(v0);

	cinematic::FreeBoxMotion2  boxMotion;

	boxMotion.set_rectangle(window);
	boxMotion.set_dimension(2, 2);
	boxMotion.set_speed(v0);

	snake::Snake::container_type							vfoods;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//			MAIN LOOP    
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	Time::Game_Timer timer;
	int count{};
	int minutes{};
	bool b_move = false;
	bool enable_collision{ false };
	bool b_grid{ false };
	bool b_enable_foods{ false };

	int Max_Game_Score = 20;

	std::wstring notify{};

	//auto vStatuBar1 = ;
	auto vStatuBar2 = TextUI::strVectorBuilder{}
		.add(L"\x25ba RIGHT")
		.add(L"\x25c4 LEFT")
		.add(L"\x25b2 UP")
		.add(L"\x25bc DOWN")
		.add(L"E Exit")
		.add(L"H Squeez")
		.add(L"G Word")
		.add(L"M Move")
		.add(L"Q Set String")
		.add(L"R Reset")
		.get();

	fVec2 mouse{};

	///////////////////////////////////////// LOOP //////////////////////////////////////////////////////
	while (konsole->is_open()) {
		konsole->clear();

		if (_snake.get_size() == Max_Game_Score)
		{
			TextUI::messageBox(cwidth / 3, cheight / 3, name1 + L" Won       ");

		}

		if (_snake2.get_size() == Max_Game_Score)
		{
			TextUI::messageBox(cwidth / 3, cheight / 3, name2 + L" Won        ");
		}


		KA_MAKE_FPS(timer);
		KA_MAKE_DALTA_TIME(1);
		minutes = static_cast<int>(timer.time_since_begin());

		while (konsole->pull_event()) {
			++count;
			mouse = fVec2(konsole->get_mouseXY());
		}

		iVec2 snake_coord = _snake.get_position().as<int>();

		PRINT_TITLE(L"Snake Application -- Kadda Aoues fps[%4.2f] minutes[%d] Mouse[%d|%d]",
			ka_fps, minutes, int(mouse.x), int(mouse.y));

		////////////////////////////////////// COMMAND ////////////////////////////////////////
		// add all alphabed randomly
		if (vfoods.empty()) {
			auto v = snake::make_Multi<size_t>(10);
			vfoods.insert(vfoods.begin(), v.begin(), v.end());
		}

		// active ghost grid
		if (KeyReleased(_u('G'))) {
			b_grid = !b_grid;
		}

		// reset 
		if (KeyReleased(_u('R'))) {
			_snake.resize(1);
			_snake2.resize(1);
			vfoods.resize(1);
		}

		// slow down
		if (KeyPressed(VK_CONTROL)) {
			if (KeyReleased(VK_DOWN)) {
				auto v = scontrol.get_speed();
				v.x -= 0.01;
				v.y -= 0.01;
				scontrol.set_speed(v);
			}

			if (KeyReleased(VK_UP)) {
				auto v = scontrol.get_speed();
				v.x += 0.01;
				v.y += 0.01;
				scontrol.set_speed(v);
			}
		}

		////////////////////////////////////// DYNAMIC //////////////////////////////////////////
		

		boxMotion(&snake, ka_Dt);

		///////////////////////////////////////////// COLLISION ////////////////////////////////
		// creation of food and eating 

		scontrol(&_snake, ka_Dt);
		collision(_snake, food, poison, stratigy_1, x, y, c);
		collision(_snake, vfoods);
		

		scontrol2(&_snake2, ka_Dt);
		collision(_snake2, food, poison, stratigy_1, x, y, c);
		collision(_snake2, vfoods);


		// set grid 
		if (b_grid) {
			auto sn = _snake.get_position().as<int>();
			Draw::box(0, sn.y, cwidth, 1, color::Black, color::BlueLight << 4);
			Draw::box(sn.x, 0, 1, cheight, color::Black, color::BlueLight << 4);
		}

		// first statu bar
		TextUI::StatuBar(TextUI::strVectorBuilder{}
			.add(L"Mouse " + std::to_wstring(int(mouse.x)) + L" " + std::to_wstring(int(mouse.y)))
			.add(L"Date " + console::make_daytime_wstring())
			.add(L"Event Count " + std::to_wstring(count))
			.add(L"Snake " + _tow(snake_coord.x) + L"|" + _tow(snake_coord.y))
			.add(name1 + L" [" + _tow(_snake.get_size()) + L"]")
			.add(name2 + L" [" + _tow(_snake2.get_size()) + L"]")
			.get(), 2);
		// second statu bar
		TextUI::StatuBar(vStatuBar2);

		for (const auto& f : vfoods) f.draw();
		poison.draw();
		food.draw();
		_snake.draw();
		_snake2.draw();
		snake.draw();

		konsole->display();
	}





	return 0;
}
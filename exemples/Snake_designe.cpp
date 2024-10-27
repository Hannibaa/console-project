#include <console project/component/utils.hpp>
#include <console project/component/TextUI/ka_StatuBar.hpp>
#include <console project/component/SnakeGame/snake.hpp>
#include <include/random/random_variable.h>

#define _tow    std::to_wstring

namespace snake {

	template<typename ...Ts>
	wstringImg make_One(const Ts&...) {
		rng::RG<char>  wrand('A', 'Z');
		std::wstring w(1, static_cast<wchar_t>(wrand()));
		RV::RVec<int>   col{ color::Red, color::Green, color::Black , color::Yellow, color::Magenta };

		return strImg::make_StringImage(w, 1, 1, col());
	}

	template<>
	wstringImg make_One<std::wstring>(const std::wstring& w) {

		RV::RVec<int>   col{ color::Red, color::Green, color::Black , color::Yellow, color::Magenta };
		return strImg::make_StringImage(w, 1, 1, col());
	}

	template<>
	std::vector<wstringImg> make_Multi<std::wstring>(const std::wstring w) {
		std::vector<wstringImg> vstr(w.size());
		int i{};
		for (auto& e : vstr) e = make_One(std::wstring(1, w[i++]));

		return vstr;
	}
}


int main() {

	const int cwidth = 145;
	const int cheight = 35;
	const int cwfont = 8;
	const int chfont = 14;
	const int index = 2;
	const float Velocity = 0.6f;
	// construct window console
	konsole->construct_console(cwidth, cheight, cwfont, chfont);
	// set drawable window
	konsole->set_drawWindows(0.f, 0.f, cwidth, cheight - index);
	// set background color
	konsole->set_background_color(color::Blue << 4);

	//// set pack element of boxes
	using Pack = utils::Pack3<100, cwstring, wsprite, wstringImg>;
	using sn_type = snake::Snake::value_type;
	Pack	pack;


	////////////////////////////// Data cwstring, wsprite, wstringImg

	fRect window(0.f, 0.f, cwidth, cheight - index);


	/////////////////////////////// Designe Snake /////////

	constexpr size_t Nelements = 50;
	fVec2 p0{ 10.f, 10.f };
	auto snake_elements = snake::make_snake<Nelements>(2, 2);
	snake::StaticSnake<Nelements> snake(snake_elements, p0);

	// andd new snake
	snake::Snake    _snake(p0);

	rng::RG<char> crand('A', 'Z');

	snake::Snake::value_type food = snake::make_One();
	snake::Snake::value_type poison = snake::make_One();


	auto stratigy_1 = [](sn_type& food, sn_type& poison, float x, float y, wchar_t w, int col) {
		food.set_position(x, y);
		food.set_string(w);
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


	// Controled motion
	cinematic::Control2 control;
	control.set_rectangle(window);
	control.set_speed(1.f);

	snake::control::SnakeControl scontrol;
	scontrol.set_rectangle(window);
	scontrol.set_speed(0.3f, 0.22f);

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

	std::wstring notify{};

	//auto vStatuBar1 = ;
	auto vStatuBar2 = TextUI::strVectorBuilder{}
		.add(L"\x25ba RIGHT")
		.add(L"\x25c4 LEFT")
		.add(L"\x25b2 UP")
		.add(L"\x25bc DOWN")
		.add(L"E Exit")
		.add(L"S Squeez")
		.add(L"G Word")
		.add(L"M Move")
		.add(L"Q Set String")
		.add(L"R Reset")
		.get();

	fVec2 mouse{};

	///////////////////////////////////////// LOOP //////////////////////////////////////////////////////
	while (konsole->is_open()) {
		konsole->clear();

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
		if (KeyReleased(_u('Q'))) {
			// add all alphabed randomly
			auto v = snake::make_Multi<std::wstring>(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
			vfoods.insert(vfoods.begin(), v.begin(), v.end());
		}

		if (KeyReleased(_u('G'))) {
			// active ghost grid
			b_grid = !b_grid;
		}



		////////////////////////////////////// DYNAMIC //////////////////////////////////////////
		scontrol(&_snake, ka_Dt);

		///////////////////////////////////////////// COLLISION ////////////////////////////////
		// creation of food and eating 
		if (physic::is_collid(_snake, food)) {
			_snake.add(food);
			notify = _snake.string();
			stratigy_1(food, poison, x(), y(), crand(), c());
			/*food.set_position(x(), y());
			food.set_string(static_cast<wchar_t>(crand()));
			food.color_bg(c());*/
		}

		if (physic::is_collid(_snake, poison)) {
			if (_snake.get_size() > 1) {
				_snake.pop();
				poison.set_position(x(), y());
			}
		}

		for (auto it = vfoods.begin(); it != vfoods.end(); ) {
			if (physic::is_collid(_snake, *it)) {
				_snake.add(*it);
				it = vfoods.erase(it);
			}
			else
				++it;
		}

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
			.add(L"String[" + notify + L"]")
			.get(), 2);
		// second statu bar
		TextUI::StatuBar(vStatuBar2);
		for (const auto& f : vfoods) f.draw();
		poison.draw();
		food.draw();
		_snake.draw();
		konsole->display();
	}





	return 0;
}
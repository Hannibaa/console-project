#include <console project/component/utils.hpp>
#include <include/string/string_utility.h>



int main() {

	// Construction of console with black background

	const int Width = 145;
	const int Hight = 35;
	const int Wfont = 8;
	const int Hfont = 14;

	konsole->construct_console(Width, Hight, Wfont, Hfont);
	konsole->set_background_color(color::Blue << 4);


	std::wstring sTime(15,L' ');
	std::wstring sCoord(10, L' ');


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    MAIN LOOP
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	fVec2 mouse{};

	Time::Game_Timer timer;
	float max_fps{};
	float min_fps{10000.f};

	std::wstring  str_body(10,L'w');

	std::vector<std::wstring>   vTitles = { L"Exit", L"F Reset" };
	
	vTitles.push_back(sTime);
	vTitles.push_back(sCoord);

	int indicator{};

	/////////////////////////////////////// Loop /////////////////////////////////////////////////////////////
	while (konsole->is_open()) {
		konsole->clear();
		
		sTime =L"Date: " + sfl::conv::str_to_wstr(make_daytime_string_s());
		vTitles[2] = sTime;

		console::Mouse mouse_event;
		// Get Event Here in Loop
		while (konsole->pull_event()) {
			++indicator;
			mouse_event = konsole->get_mouse();
			mouse.x = mouse_event.x;
			mouse.y = mouse_event.y;
		}

		// reset maximum fps
		if (KeyPressed(_u('R'))) {
			indicator = 0;
			max_fps = 0.f;
			min_fps = ka_fps;
		}
		
		// check of event of mouse:
		if (mouse_event.button == console::Mouse::Button::LCtrl)
		{
			str_body = L"Action mouse and Ctrl is On";
		}

		//if (console::Mouse::is_buttonPressed(mouse_event.button)) {

		//	if (mouse_event.motion == console::Mouse::Motion::Pressed)
		//	{
		//		str_body = L"Button mouse clicked";
		//	}


		//	if (mouse_event.motion == console::Mouse::Motion::Released) {
		//		str_body = L"Button mouse released";
		//	}
		//}

		if (mouse_event.event == console::Mouse::Event::DoubleClick)
		{
			str_body = L"Double click";
		}

		if (mouse_event.button == console::Mouse::Button::WheelDown)
		{
			str_body = L"Wheel Down";
		}

		if (mouse_event.button == console::Mouse::Button::WheelUp)
		{
			str_body = L"Wheel Up";
		}
		

		// title and main parameter fps, Dt60, Dt, ...
		KA_MAKE_FPS(timer);
		KA_MAKE_DALTA_TIME(1.0f);

		console::get_maximum(max_fps, ka_fps);
		console::get_minimum(min_fps, ka_fps);

		PRINT_TITLE(L"Application[%d][x:%3.2f | y:%3.2f] fps[%4.2f|%4.2f|%4.2f] elapse[%2.5f] Dt[%5.5f]",
			indicator,mouse.x, mouse.y,
			ka_fps, max_fps,min_fps,  ka_elaps, ka_Dt);


		sCoord =L"Mouse: " + std::to_wstring(int(mouse.x)) + L" " +
			std::to_wstring(int(mouse.y));
		vTitles[3] = sCoord;
		// display message box that contain all event of mouse:
		console::Message_Box(console::BuildMessage{}
			.set_position(konsole->width() / 3, konsole->hight() / 3)
			.set_title(L"Mouse Event")
			.set_background_color(color::Blue << 4)
			.set_border(console::Hborder)
			.set_border_color(color::Red)
			.set_title_color(color::White)
			.set_body_color(color::White)
			.set_body(str_body)
			.get());

		console::SubMenu(0, Hight - 1,3, vTitles, COLOR::FG_YELLOW, COLOR::FG_DARK_YELLOW << 4, color::Red);
		konsole->display();
	}





	return 0;
}
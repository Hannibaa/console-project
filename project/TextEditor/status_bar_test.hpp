#pragma once
#include <map>
#include <vector>
#include <include/string/string_utility.h>
#include <include/time/Game_timer.h>
#include <console project/include/component/TextUI/StatuBar/ka_StatusBar2.hpp>
#include <console project/include/component/ka_SpriteString.hpp>
#include <console project/include/component/ka_SString.hpp>

#define _INVERT(bvalue)     bvalue = !bvalue

struct Option
{
	bool			action;
	std::wstring	comment;
};

using CustomOption = std::map<std::string, Option>;

std::wstring process_option(const CustomOption& option, std::wstring& comment)
{

	for (const auto& pair : option)
	{
		if (pair.second.action)
		{
			if (pair.first == "ShowTime") comment = L"Show a current time";
			if (pair.first == "ShowDate") comment = L"Show a today date";
			if (pair.first == "ShowCursor") comment = L"Show a coordinate mouse";
		}
	}

	return comment;
}

int game(int W, int H, int fw, int fh)
{
	int level = 2;
	int ground_color = color::Blue << 4;

	konsole->construct_console(W, H, fw, fh);
	konsole->set_background_color(ground_color);
	konsole->set_drawWindows(0.f, 0.f, W, H - level);

	/// ----------------- Option ---------------------------------------------------

	CustomOption option;

	option["Time"] = { false, L"Show current time" };
	option["Date"] = { false, L"Show today date" };
	option["Cursor"] = { false, L"Show coordinate mouse" };
	option["Keyboard"] = { false, L"Show key clicked" };

	/// ----------------- Data -----------------------------------------------------


	/// ---------------- Main Loop --------------------------------------------------
	
	Time::Game_Timer	timer;
	fVec2				mouse;
	fVec2				pt{};
	bool				bDisplay{ false };
	wchar_t				key{};

	int					_color[] = { color::GrayLight, color::GrayDark };
	std::wstring		_action(8,L' ');

	while (konsole->is_open())
	{
		konsole->clear();

		KA_MAKE_FPS(timer);
		KA_MAKE_DALTA_TIME(1.0f);

		while (konsole->pull_event()) {
			mouse = geom2d::as<float>(konsole->get_mouseXY());

			key = konsole->get_AsciiKey();
		}

		PRINT_TITLE(L"Program fps[%4.2f] [%f]", ka_fps, ka_Dt);

		// ----- 

		if (KeyReleased(_u('T'))) {
			option["Time"].action = ! option["Time"].action;
			bDisplay = true;
		}

		if (KeyReleased(_u('D'))) {
			option["Date"].action = !option["Date"].action;
			bDisplay = true;
		}

		if (KeyReleased(_u('C'))) {
			option["Cursor"].action = !option["Cursor"].action;
			bDisplay = true;
		}

		if (KeyPressed(VK_CONTROL))
			if (KeyReleased(_u('K'))) {
				option["Keyboard"].action = !option["Keyboard"].action;
				bDisplay = true;
			}


		std::wstring sTime(10,L' ');
		std::wstring sDate(10, L' ');
		if (option["Date"].action)
		{
			sDate = console::make_current_date();
			if (bDisplay)
			{
				_action = option["Date"].comment;
				bDisplay = false;
			}
		}

		if (option["Time"].action) 
		{
			sTime = console::make_current_time();
			if (bDisplay)
			{
				_action = option["Time"].comment;
				bDisplay = false;
			}

		}


		std::wstring cursor(9,L' ');
		if (option["Cursor"].action)
		{
			swprintf_s(cursor.data(), 8, L"%3d|%2d", int(mouse.x), int(mouse.y));

			if (bDisplay)
			{
				_action = option["Cursor"].comment;
				bDisplay = false;
			}
		}
		std::wstring strkey(2, L' ');
		if (option["Keyboard"].action)
		{
			strkey = std::wstring(1, key);

			if (bDisplay)
			{
				_action = option["Keyboard"].comment;
				bDisplay = false;
			}
		}

		//process_option(option, _action);

		TextUI::statu_bar(TextUI::StatuBarBuilder{}
			.set_position(0, H - level)
			.set_colorBG(color::GrayLight)
			.set_colorFG(color::Black)
			.set_colorChar(color::Red)
			.set_colorbg_string(color::YellowLight)
			.set_color_select(color::GrayDark)
			.set_step(1)
			.add_item(L"Time",		0 , option["Time"].action )
			.add_item(L"Date",		0 , option["Date"].action )
			.add_item(L"Cursor",	0 , option["Cursor"].action)
			.add_item(L"^Keyboard",	0 , option["Keyboard"].action,0,2)
			.add_item(cursor,W-7, false,3)
			.add_item(strkey,W-9,false)
			.add_item(sDate,W - sDate.size() - 10, false,0,3 )
			.add_item(sTime,W - sTime.size() - 26, false,0,2)
			.add_item(L"Max Fps",0,false, 4,3)
			.get());
		
		TextUI::statu_bar(TextUI::StatuBarBuilder{}
			.set_position(0, H - level + 1)
			.set_colorBG(color::Black)
			.set_colorFG(color::White)
			.set_colorChar(color::White)
			.add_item(L"Command : ")
			.add_item(_action)
			.get());

		konsole->display();
	}



	return 0;
}
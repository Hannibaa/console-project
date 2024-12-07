/*
				Implemetation of basic nessecary for Statu Bar 
				Author		:	Kadda Aoues
				Date		:	04 / 12 / 2024
				Name		:	Basic Nessicisary for StatuBar
				History		:	
				Version		:	0.1.0
				Discription	:	as in commentary below, 
								map of option named : option


				TODO :
				------
				1. Implement command sender		-- command function
				2. Implement command parser		-- command history
				3. Implement command executor	-- function executor.
				4. Implement text traitor
				5. Add max fps and min fps 

*/


#pragma once
#include <map>
#include <vector>
#include <queue>
#include <include/string/string_utility.h>
#include <include/time/Game_timer.h>
#include <console project/include/component/TextUI/StatuBar/ka_StatusBar2.hpp>
#include <console project/include/component/ka_SpriteString.hpp>
#include <console project/include/component/ka_SString.hpp>
#include <console project/include/component/TextUI/Palette/ka_Palette16.hpp>

#define  _DISPLAY_COMMENT(option_name)	display_comment(bControl,option[#option_name], comment)

#define _INVERT_OPTION(option_name)	option[#option_name].action = ! option[#option_name].action

#define _INVERT(bvalue)     bvalue = !bvalue

// Function : write format precision of float %n.mf to return string
// exemple  : 345.44454 with 5.2f = |  345.44| or after : |345.44  |
std::wstring float_format(float f, const int decimal)
{
	std::wstring strf = std::to_wstring(f);
	size_t size = strf.size();
	std::wstring format = L"%." + std::to_wstring(decimal) + L"f";
	swprintf_s(strf.data(), size, format.c_str() , f);

	sfl::trait::remove_char<wchar_t, sfl::BEGINING_AND_END>(strf,
		+[](int x)->int {return !std::iswprint(x) | std::iswblank(x); });

	return strf;
}


// Function : display comment of command or option 
void display_comment(bool& bControl, TextUI::wsOption option, std::wstring& comment)
{
	bControl = true;
	if (bControl && option.action)
	{
		comment = option.comment;
		bControl = false;
	}
}

// Function return for only fally printed string
bool is_printedstr(const std::wstring& str) {
	for (const auto& w : str)
		if (std::iswgraph(w)) return true;
	return false;
}

// Function : get command from keyboard and placed in queue
void get_command_line(std::queue<std::wstring>& qCommand, 
	std::wstring& strcommand ,wchar_t input_key, size_t command_size)
{
	if (input_key == KA_BACKSPACE)
	{
		if (!strcommand.empty())
			strcommand.pop_back();
	}
	else if (input_key == KA_CR)
	{
		if (!strcommand.empty() && is_printedstr(strcommand))
		{
			sfl::trait::remove_char<wchar_t, sfl::BEGINING_AND_END>(strcommand,
				+[](int x)->int {return std::iswcntrl(x) | std::iswblank(x); });
			qCommand.push(strcommand);
		}
		strcommand.clear();
	}
	else
	{
		strcommand.push_back(input_key);
	}

	if (strcommand.size() > command_size) strcommand.clear();
}

// Function : display history command line
void history_command(std::queue<std::wstring> qCommand)
{
	int i = 0;
	int j = -1;
	while (!qCommand.empty())
	{
		Draw::draw_str(i, ++j, qCommand.front(), color::White, konsole->get_background_color() >> 4);
		qCommand.pop();
	}
}

void clear(std::queue<std::wstring>& qCommand) {
	if (!qCommand.empty()) qCommand.pop();
}

int game(int W, int H, int fw, int fh)
{
	int level = 2;
	int ground_color = color::Blue << 4;

	const wchar_t* APP_NAME = L"Statu Bar Testing";
	int MAX_COMMAND_SIZE = 15;

	konsole->construct_console(W, H, fw, fh);
	konsole->set_background_color(ground_color);
	konsole->set_drawWindows(0.f, 0.f, W, H - level);

	/// ----------------- Command -------------------------------------------------------
	std::queue<std::wstring> qCommand;

	/// ----------------- Option -------------------------------------------------------
	bool							bControl{ false };		// should be named bControl.
	std::wstring					comment;				// should be comment.
	TextUI::mapOption<wchar_t>		option;					// should be option.			

	option["Time"]		= { false, L"Show current time" };
	option["Date"]		= { false, L"Show today date" };
	option["Cursor"]	= { false, L"Show coordinate mouse" };
	option["Keyboard"]	= { false, L"Show key clicked" };
	option["Type"]		= { false, L"Enter words separated by entry"};
	option["MaxFps"]	= { false, L"Display maximum fps"};
	option["MinFps"]	= { false, L"Display minimum fps"};
	option["History"]	= { false, L"Command history"};
	option["ResetMM"]	= { false, L"Reset fps maximum and minimum"};
	option["Palette"]	= { false, L"Chose color from palette"};

	/// ----------------- Data -----------------------------------------------------
	TextUI::Palette16		palette(10,10);


	/// ---------------- Main Loop --------------------------------------------------
	
	Time::Game_Timer	timer, timer2;
	fVec2				mouse;
	fVec2				pt{};
	bool				bDisplay{ false };
	wchar_t				key{};

	float max_fps{};
	float min_fps{ 10000.f };

	int					_color[] = { color::GrayLight, color::GrayDark };
	int reset_count{};
	std::wstring		strcommand;
	int col_select{15};

	//////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////-- LOOP --///////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	while (konsole->is_open())
	{
		konsole->clear();

		KA_MAKE_FPS(timer);
		KA_MAKE_DALTA_TIME(1.0f);

		console::get_maximum(max_fps, ka_fps);
		console::get_minimum(min_fps, ka_fps);


		wchar_t KeyIn{};
		while (konsole->pull_event()) {
			mouse = geom2d::as<float>(konsole->get_mouseXY());

			key = konsole->get_AsciiKey();
			KeyIn = key;

			// making text
			if (konsole->key_pressed())
			{

				// some command need key pressed 
				// -----------------------------
				if (KeyIn == KA_CTRL_KEY('P')) {
					_INVERT_OPTION(Type);
					_DISPLAY_COMMENT(Type);
				}

				if (KeyIn == KA_CTRL_KEY('T')) {
					_INVERT_OPTION(Time);
					_DISPLAY_COMMENT(Time);
				}

				if (KeyIn == KA_CTRL_KEY('D')) {
					_INVERT_OPTION(Date);
					_DISPLAY_COMMENT(Date);
				}

				if (KeyIn == KA_CTRL_KEY('C')) {
					_INVERT_OPTION(Cursor);
					_DISPLAY_COMMENT(Cursor);
				}

				if (KeyIn == KA_CTRL_KEY('K')) {
					_INVERT_OPTION(Keyboard);
					_DISPLAY_COMMENT(Keyboard);
				}

				if (KeyIn == KA_CTRL_KEY('H')) {
					_INVERT_OPTION(History);
					_DISPLAY_COMMENT(History);
				}

				if (KeyIn == KA_CTRL_KEY('O'))
				{
					clear(qCommand);
				}

				// reset maximum fps
				if (KeyIn == KA_CTRL_KEY('R')) {
					_INVERT_OPTION(ResetMM);
					_DISPLAY_COMMENT(ResetMM);
					max_fps = 0.f;
					min_fps = ka_fps;
				}


				// function to precess command line
				if (option["Type"].action)
					get_command_line(qCommand, strcommand, key, MAX_COMMAND_SIZE);

			}// End of key pressed

		}

		// printing in title of windows
		std::wstring strcol = (col_select < 16 && col_select > -1) ?
			color::wPalette16[col_select] : std::wstring(L"No Selection");

		PRINT_TITLE(L"-- %s -- fps[%4.2f] [%f] -- CommandBuffer[%d]color[%s]"
			,APP_NAME,  ka_fps, ka_Dt , qCommand.size(), strcol.c_str());

		

		// Display all command history color::wPalette16[col_select].c_str()
		if (option["History"].action)
		{
			history_command(qCommand);
		}


		if (!qCommand.empty())
		{

			if (qCommand.back() == L"Max")
			{
				_INVERT_OPTION(MaxFps);
				_DISPLAY_COMMENT(MaxFps);
				qCommand.push(L"Empty Command");
			}

			if (qCommand.back() == L"Min")
			{
				_INVERT_OPTION(MinFps);
				_DISPLAY_COMMENT(MinFps);
				qCommand.push(L"Empty Command");
			}

			if (qCommand.back() == L"Palette")
			{
				_INVERT_OPTION(Palette);
				_DISPLAY_COMMENT(Palette);
				qCommand.push(L"Empty Command");
			}

		}

		// reset every 5 min
		if (option["ResetMM"].action) {
			if (timer2.get_elapsed_time() > 5.f)
			{
				max_fps = 0.f;
				min_fps = ka_fps;
				timer2.reset();
				++reset_count;
			}
		}

		std::wstring strMaxFps(1,L' ');
		if (option["MaxFps"].action)
		{
			strMaxFps = float_format(max_fps, 2);
		}

		std::wstring strMinFps(1, L' ');
		if (option["MinFps"].action)
		{
			strMinFps = float_format(min_fps, 2);
		}

		std::wstring sTime(10,L' ');
		std::wstring sDate(10, L' ');
		if (option["Date"].action)
		{
			sDate = console::make_current_date();
		}

		if (option["Time"].action) 
		{
			sTime = console::make_current_time();
		}


		std::wstring cursor(9,L' ');
		if (option["Cursor"].action)
		{
			swprintf_s(cursor.data(), 8, L"%3d|%2d", int(mouse.x), int(mouse.y));
		}

		std::wstring strkey(2, L' ');
		if (option["Keyboard"].action)
		{
			strkey =L"[" + std::to_wstring(int(key)) + L"]";
		}

		std::wstring text;
		if (option["Type"].action)
			text = strcommand;
		else
			text = comment;

		// Drawing palette
		if (option["Palette"].action)
		{
			palette.draw();
			palette.select_color(col_select ,mouse.as<int>(), option["Palette"].action);
		}


		// Statu Bar 
		TextUI::statu_bar(TextUI::StatuBarBuilder{}
			.set_position(0, H - level)
			.set_colorBG(color::GrayLight)
			.set_colorFG(color::Black)
			.set_colorChar(color::Red)
			.set_colorbg_string(color::YellowLight)
			.set_color_select(color::GrayDark)
			.set_step(1)
			.add_item(L"^Time",		0, option["Time"].action		,0,2)
			.add_item(L"^Date",		0, option["Date"].action		,0,2)
			.add_item(L"^Cursor",	0, option["Cursor"].action		,0,2)
			.add_item(L"^Keyboard",	0, option["Keyboard"].action	,0,2)
			.add_item(L"Ty^Ping",	0, option["Type"].action		,2,2)
			.add_item(L"MaxFps",	0, option["MaxFps"].action		,4,3)
			.add_item(strMaxFps,	0, false						,0,0)
			.add_item(L"MinFps",	0, option["MinFps"].action		,4,3)
			.add_item(strMinFps,	0, false						,0,0)
			.add_item(L"Palette",	0, option["Palette"].action		,0,0)
			.add_item(L"^Reset",	0, option["ResetMM"].action		,0,2)
			.add_item(L"^History",	0, option["History"].action		,0,2)
			.add_item(cursor	,W-7					,false		,3	)
			.add_item(strkey	,W-11					,false			)
			.add_item(sDate		,W - sDate.size() - 12	,false		,0,3)
			.add_item(sTime		,W - sTime.size() - 28	,false		,0,2)
			.get());
		
		TextUI::statu_bar(TextUI::StatuBarBuilder{}
			.set_position(0, H - level + 1)
			.set_colorBG(color::Black)
			.set_colorFG(col_select)
			.set_colorChar(color::Red)
			.add_item(L"Command : ")
			.add_item(text)
			.get());

		konsole->display();
	}



	return 0;
}
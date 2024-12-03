﻿#pragma once
#include <vector>
#include <map>
#include <include/random/random_generator.h>
#include <include/console/ka_utility.hpp>
#include <include/component/ka_SpriteString.hpp>
#include <include/component/ka_SString.hpp>
#include <include/component/Physics/ka_Kinematics.h>
#include <include/component/TextUI/StatuBar/ka_StatusBar2.hpp>
#include <include/component/Physics/ka_Physics.hpp>

#define _tow(val)			std::to_wstring(val)
#define _INVERT(bvalue)		bvalue = !bvalue

// structure for option
struct Coption
{
	bool			action;
	std::wstring	comment;
};

using Option = std::map<std::string, Coption>;

// using as sprite for any drawing :
using Sprite = console::SpriteString<wchar_t>;

// using vector of sprite
using vecSprite = std::vector<Sprite>;

using strButton = WSString;
using vecString = std::vector<std::wstring>;
// ID for all object sprite
int ID{};

// Function to make box string represented lx and ly dimension.
std::wstring make_box_string(int lx, int ly, const wchar_t (&border)[6] ) {

	if (lx == 1 && ly == 1) return std::wstring(1, 0x2588);

	if (lx == 1) return std::wstring(ly, console::Hborder[3]);
	if (ly == 1) return  std::wstring(lx,console::Hborder[1]);
		

	std::wstring str;
	str.resize(lx * ly, L' ');


	str[0] = border[0];
	str[lx - 1] = border[2];
	str[(ly - 1) * lx] = border[5];
	str[(ly - 1) * lx + lx - 1] = border[4];

	for (int i = 1; i < lx - 1; ++i)
	{
		str[i] = border[1];
		str[(ly - 1) * lx + i] = border[1];
	}

	for (int j = 1; j < ly - 1; ++j)
	{
		str[j * lx] = border[3];
		str[j * lx + lx - 1] = border[3];
	}

	return str;
}

Sprite make_random_box( int W, int H, int rx, int ry)
{
	Sprite sp;

	rng::iRG<int> irand;

	int lx = irand(1, W);
	int ly = irand(1, H);

	sp.set_string(make_box_string(lx, ly, console::Hborder));
	sp.set_size(lx, ly);
	sp.set_position(irand(0,rx), irand(0,ry));
	sp.color_bg(konsole->get_background_color()>>4);
	sp.color_fg(color::Red);
	sp.set_id(++ID);

	return sp;
}

Sprite make_box(const fVec2& p1, const fVec2& p2, int _col = 0) {
	Sprite sp;

	int lx = std::abs(p1.x - p2.x);
	int ly = std::abs(p1.y - p2.y);

	//int x = p1.x > p2.x ? p1.x : p2.x;
	//int y = p1.y > p2.y ? p1.y : p2.y;

	sp.set_string(make_box_string(lx, ly, console::Hborder));
	sp.set_size(lx, ly);
	sp.set_position(p1.x, p1.y);
	sp.color_bg(konsole->get_background_color() >> 4);
	sp.color_fg(color::Red);
	sp.set_id(++ID);

	return sp;
}

void get_2points(iVec2& p1, iVec2& p2, bool& bControl,const fVec2& mouse, short Key)
{
	if (KeyPressed(Key)) {
		if (bControl) {
			p1 = mouse.as<int>();
			bControl = false;
		}

		p2 = mouse.as<int>();
	}
	else
		bControl = true;

}



// principal game:
int game(int W, int H, int fw, int fh, int background_color = color::Blue) {

	// this for statu bar
	int level = 2;
	const wchar_t* APP_TITLE = L"Random Box";

	konsole->construct_console(W, H, fw, fh);
	konsole->set_background_color(background_color << 4);
	konsole->set_drawWindows(0, 0, W, H - level);

	win::Set_ConsoleWindow_Style();
	win::move_console_window(50, 50);

	/// ------------------- 0. Option

	Option  option;

	option["Time"] = { false, L"show current time" };
	option["Date"] = { false, L"show today date" };
	option["Cursor"] = { false, L"show cursor coordinate" };
	option["Keyboard"] = { false, L"show key clicked" };
	option["Type"] = { false, L"show taping word, use space to delate" };
	option["Select"] = { false, L"select an object by mouse" };
	option["Add"] = { false, L"add an object to a vector" };
	option["Draw"] = { false, L"Enable drawing" };
	option["Collision"] = { false, L"Activate collision mode" };

	/// ------------------- 1. Data

	int lx{6};
	int ly{3};
	fVec2 p0(40.f, 10.f);
	bool b_On;	
	
	vecSprite  vec;
	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//   Main Loop    
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	Time::Game_Timer timer;
	fVec2			 mouse;
	iVec2			 imouse;
	iVec2			 p1{};
	iVec2			 p2{};
	iVec2			 pp1{};
	iVec2			 pp2{};
	wchar_t			 key{};
	const size_t	 MAX_STRING_SIZE = 14;
	int				 count{};
	int				 _id{};
	std::wstring strword(10, L' ');

	while (konsole->is_open()) {

		konsole->clear();

		// primitive parameter of konsole
		KA_MAKE_FPS(timer);
		KA_MAKE_DALTA_TIME(1.f);

		PRINT_TITLE(L"%s fps[%4.2f][%1.4f] Event[%d] NumberObj[%d]", 
			APP_TITLE, ka_fps, ka_Dt, count, vec.size());

		while (konsole->pull_event()) {
			++count;
			mouse = geom2d::as<float>(konsole->get_mouseXY());
			imouse = mouse.as<int>();
			key = konsole->get_AsciiKey();

			if (konsole->key_released())
			strword.push_back(key);

			if (std::iswblank(key))
				strword.clear();

			if (strword.size() > MAX_STRING_SIZE)
				strword.clear();
		}

		// Using Control CRTL ^

		if (KeyPressed(VK_CONTROL))
		{
			if (KeyReleased(_u('T')))
			{
				_INVERT(option["Time"].action);
			}

			if (KeyReleased(_u('D')))
			{
				_INVERT(option["Date"].action);
			}

			if (KeyReleased(_u('C')))
			{
				_INVERT(option["Cursor"].action);
			}

			if (KeyReleased(_u('K')))
			{
				_INVERT(option["Keyboard"].action);
			}

			if (KeyReleased(_u('P')))
			{
				_INVERT(option["Type"].action);
			}

			if (KeyReleased(_u('R')))
			{
				_INVERT(option["Draw"].action);
			}
		}

		// ---------------------- option implementation
		std::wstring sTime(10, L' ');
		std::wstring sDate(10, L' ');
		if (option["Date"].action)
		{
			sDate = console::make_current_date();
		}

		if (option["Time"].action)
		{
			sTime = console::make_current_time();
		}

		std::wstring cursor(9, L' ');
		if (option["Cursor"].action)
		{
			swprintf_s(cursor.data(), 8, L"%3d|%2d", imouse.x, imouse.y);
		}

		std::wstring strkey(3, L' ');
		if (option["Keyboard"].action)
		{
			strkey = L"[" + std::wstring(1, key) + L"]";
		}

		
		if (!option["Type"].action)
		{
			strword = L" ";
		}

		if (option["Draw"].action)
		{


			if (KeyReleased(_u('A')) && !option["Select"].action)
			{
				vec.push_back(make_random_box(5, 5, W -5,H-5-level));
			}

			if (KeyReleased(_u('D'))) {
				if (p1 != p2) {
					auto p = p1 - iVec2(1, 1);
					vec.push_back(make_box(p.as<float>(), pp2.as<float>(), color::White));
					p1 = p2;
				}
			}

			if (KeyReleased(_u('C'))) {
				_INVERT(option["Collision"].action);
			}

			if (KeyReleased(_u('S'))) {
				_INVERT(option["Select"].action);
			}

			// TODO selection of boxes
			if (option["Select"].action) {
				for (auto it = vec.begin(); it != vec.end(); ++it) {
					if (it->get_bounds().contain(mouse))
					{
						it->color_bg(color::GrayDark);
						_id = it->get_id();
						break;
					}
					else {
						it->color_bg(background_color);
						_id = 0;
					}
				}
			}

			if (!option["Select"].action) {
				get_2points(p1, p2, b_On, mouse, VK_LBUTTON);
				pp1 = p1 - iVec2(1, 1);
				pp2 = p2 + iVec2(1, 1);
			}


		}

		//كريم مع ادم hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
		// add collision
		// TODO : NEED TO UPDATE get_bounds() functions to get right rectangle

		if (option["Collision"].action) {
			physic::collision_process(vec);
		}

		// Show two points coordinate in statu bar
		std::wstring strP1(7, L' ');
		std::wstring strP2(7,L' ');
		if (option["Draw"].action && !option["Select"].action)
		{
			swprintf_s(strP1.data(), 7, L"%3d|%2d", p1.x, p1.y);
			swprintf_s(strP2.data(), 7, L"%3d|%2d", p2.x, p2.y);
		}

		// show id of boxes selected
		std::wstring str_id(2, L' ');
		if (option["Select"].action)
		{
			str_id = L"Id[" + _tow(_id) + L"]";
		}

		// State Statu bar 
		TextUI::statu_bar(TextUI::StatuBarBuilder{}
			.set_position(0, H - level + 1)
			.set_colorBG(color::GrayLight)
			.set_colorFG(color::Black)
			.set_colorChar(color::Red)
			.set_color_select(color::GrayDark)
			.set_step(1)
			.add_item(L"^Time", 0, option["Time"].action, 0, 2)
			.add_item(L"^Date", 0, option["Date"].action, 0, 2)
			.add_item(L"^Cursor", 0, option["Cursor"].action, 0, 2)
			.add_item(L"^Keyboard", 0, option["Keyboard"].action, 0, 2)
			.add_item(L"Ti^Ping", 0, option["Type"].action, 2, 2)
			.add_item(cursor, W - 7, false, 3)
			.add_item(sDate, W - sDate.size() - 8, false, 0, 3)
			.add_item(sTime, W - sTime.size() - 24, false, 0, 2)
			.add_item(strkey, W - 36, false, 1, 1)
			.add_item(strword, W - 47, false)
		.get());

		// Second Statu Bar for Drawing Option
		TextUI::statu_bar(TextUI::StatuBarBuilder{}
			.set_position(0, H - level)
			.set_colorBG(color::GrayLight)
			.set_colorFG(color::Black)
			.set_colorChar(color::Red)
			.set_color_select(color::GrayDark)
			.set_step(1)
			.add_item(L"D^raw", 0, option["Draw"].action, 1,2)
			.add_item(L"Select", 0, option["Select"].action)
			.add_item(L"Add")
			.add_item(L"Collision", 0 , option["Collision"].action)
			.add_item(strP1, 0, false, 3)
			.add_item(strP2, 0, false, 3)
			.add_item(str_id, 0, false,1,4)
		.get());

		// Option Statu bar
		//TextUI::StatuBar(TextUI::StatuBarBuilder{}
		//	.set_position(0, H - 2)
		//	.set_colorBG(color::Yellow << 4)
		//	.set_colorFG(color::White)
		//	.set_colorChar(color::Red)
		//	.set_step(2)
		//	.add_item(L"P1 : " + _tow(p1.x) + L"|" + _tow(p1.y))
		//	.add_item(L"P2 : " + _tow(p2.x) + L"|" + _tow(p2.y))
		//	.add_item(L"Size vec " + _tow(vec.size()))
		//	.add_item(L"Mode[" + Mode + L"]")
		//	.add_item(L"Collision[ " + Mode_collision + L" ]")
		//	.get());

		for (const auto& s : vec) s.draw();

		if (option["Draw"].action)
			Draw::border_box(console::Hborder, p1, pp2, color::White, background_color);


		konsole->display();
	}






	return 0;
}
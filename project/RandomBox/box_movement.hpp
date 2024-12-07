/*				    Box Movement Demo
*					Author :	Kadda Aoues
*					Name :		Box Movement Demo
*					Discription :
*					Date :		11 / 10 / 2024
* 
					Enum : Mode { Creation, Selection, ... }
						 : One Box creation methode
					TODO :
					------

					1. fix option issur by new methode of statu bar and options 
					2. add new box creation 
					3. modifing box shape
					4. add colors and glyphs, name
					5. add palette to chose colors
*/
#pragma once
#include <vector>
#include <map>
#include <include/console/ka_utility.hpp>
#include <include/component/ka_SpriteString.hpp>
#include <include/component/ka_SString.hpp>
#include <include/component/TextUI/StatuBar/ka_StatusBar2.hpp>
#include <include/component/Physics/ka_Physics.hpp>
#include <include/component/ka_Draw.hpp>
#include <include/component/Physics/ka_FreeMove.hpp>
#include <include/component/Physics/ka_ControlMove.hpp>
#include <include/component/Physics/ka_Parametric_coord.hpp>
#include <include/component/Physics/ka_Jump.hpp>
#include <include/component/Physics/ka_ContinusMove.hpp>
#include <include/component/TextUI/Palette/ka_Palette16.hpp>

#define _tow(val)			std::to_wstring(val)

#define _DISPLAY_COMMENT(option_name)	display_comment(bControl,option[#option_name], comment)

#define _INVERT_OPTION(option_name)		option[#option_name].action = ! option[#option_name].action

#define _INVERT(bvalue)					bvalue = !bvalue


using Sprite = console::SpriteString<wchar_t>;
using vecSprite = std::vector<Sprite>;

using strButton = WSString;
using vecString = std::vector<std::wstring>;

//using MapBool = std::map<std::string, bool>;

using IMove = physic::MoveInterface0;

struct sStringBox {
	std::wstring _str;
	int _lx;
	int _ly;
};

sStringBox make_box_string(int lx, int ly, const wchar_t(&border)[6]) {

	if (lx == 1 && ly == 1) return sStringBox{ std::wstring(1, 0x2588), 1, 1 };

	if (lx == 1) return sStringBox{ std::wstring(ly, console::Hborder[3]), 1, ly };
	if (ly == 1) return  sStringBox{ std::wstring(lx, console::Hborder[1]), lx, 1 };


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

	return sStringBox{ str, lx , ly };
}

void all_false(bool** option, int number_option) {
	for (int i = 0; i < number_option; ++i) *option[i] = false;
}

void all_false(TextUI::mapOption<wchar_t>& options,const std::vector<std::string>& str_options)
{
	for (const auto str : str_options)
		if (options.contains(str)) options[str].action = false;
}

int game(int W, int H, int fw, int fh, int background_color = color::Blue) {

	const int level = 3;
	int		  bg_color{ background_color };
	const wchar_t* APP_NAME = L"Box Movement Test";
	const float speed_60 = 0.01667f;
	const fVec2 v60(0.5, 0.5);

	konsole->construct_console(W, H, fw, fh);
	konsole->set_background_color(background_color << 4);
	konsole->set_drawWindows(0.f, 0.f, W, H - level);

	// set windows console static
	win::Set_ConsoleWindow_Style();
	// set window console position
	win::move_console_window(50, 50);

	/// -------------------------------- Option ----------------------------------------
	TextUI::mapOption<wchar_t>	option;
	std::vector<std::string>	strMovementOptions;
	std::wstring				comment;

#define  MoveOptions(name,comment)		option[#name]		= { false, L#comment};\
										strMovementOptions.push_back(#name)

	option["Move"]		= { false, L"Enable Movement"};
	option["Display"]	= { false, L"Display sprite string drawing"};
	MoveOptions(Control,		Using arraw to control movement);
	MoveOptions(Jump,			Jumping movement);
	MoveOptions(Parametric,		Parametric definition movement);
	MoveOptions(Free,			Free straight move);
	MoveOptions(Falldown,		Free fall movement);
	MoveOptions(Move2points,	Moving between two points redefined);
	MoveOptions(MoveAB,			Move particule from A to B);
	MoveOptions(Continus,		Continus movement of particule);

	option["Get2Point"]			= { false, L"Get two point by mouse with Ctrl + LButton"};
	option["Palette"]			= { false, L"Display color palette"};

	/// -------------------------------- End Option ----------------------------------------


	// --------------------------------- data here --------------------------------------
	auto str_box = make_box_string(1, 1, console::Hborder);

	const int color_box_bg = background_color;
	int color_box_fg = color::Red;

	Sprite  sprite(str_box._str,color_box_fg, color_box_bg);

	sprite.set_size(str_box._lx, str_box._ly);
	sprite.set_position(100.f, 10.f);
	sprite.color_bg(color_box_bg);
	sprite.color_fg(color_box_fg);
	
	// Palette Object 
	TextUI::Palette16 palette(100, 5);

	// --------------------------------- End Data --------------------------------------


	//////////////////////////////////////// Cinematics /////////////////////
	IMove* movement = nullptr;

	auto drawWindow = iRect{ 0,0 ,W, H - level }.as<float>();
	// controlling object by arrow key //////////////////////
	physic::ControlMove<Sprite> control;
	control.set_velocity(v60);
	control.set_space(drawWindow);
	// free move in box /////////////////////////////////////
	physic::FreeMove0<Sprite> freemove;
	freemove.set_velocity(v60);
	freemove.set_space(drawWindow);

	auto X = [](float x) {float a = 70.f; float b = 15.f;
							return a + b * std::sinf(3.14f * 2.f * x); };

	auto Y = [](float x) {float a = 15.f; float b = 15.f;
							return a + b * std::cosf(3.14f * 2.f * x); };

	auto Xe = [&sprite](float x) { return sprite.get_position().x; };
	auto Ye = [&sprite](float x) {float g = 9.8f, v0 = 0.001f, y_ground = sprite.get_position().y;
		 return 0.5f * g * x * x - v0 * x + y_ground; };

	// parametrical move ///////////////////////////////////
	physic::ParaMove<Sprite> paramove(X,Y);
	paramove.set_speed(0.01667);
	paramove.set_space(drawWindow);
	// Jumping //////////////////////////////////////////////
	physic::Jump<Sprite>   jump;
	jump.set_velocity(0.6f, 0.7f);
	jump.set_space(drawWindow);
	// Moving between two points ////////////////////////////
	physic::Move2p<Sprite>   move2p;
	move2p.set_space(drawWindow);
	move2p.set_velocity(0.01f, 0.01f);
	move2p.set_2points({ 1.f, 1.f }, { 100.f, 30.f });
	// following mouse technic //////////////////////////////
	physic::MoveAB<Sprite> move2points;
	move2points.set_period(1.0f);
	move2points.set_speed(0.08f);
	// Continus movement controled by arrow direction ///////
	physic::ContinusMove<Sprite>	continus;
	continus.set_velocity(0.5f, 0.5f);
	continus.set_sequeezKey('X');
	continus.noreturn(true);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//   Main Loop    
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	Time::Game_Timer timer, Seconds;
	fVec2			 mouse;
	int				 ground{};

	while (konsole->is_open()) {

		konsole->clear();

		// primitive parameter of konsole 1 step every frame 60/s
		KA_MAKE_FPS(timer);
		KA_MAKE_DALTA_TIME(1.f);

		PRINT_TITLE(L"-- %s -- fps[%4.2f][%1.4f] Seconds[%d]"
			,APP_NAME, ka_fps, ka_Dt, (int) Seconds.get_elapsed_time() );
		while (konsole->pull_event()) {
			mouse = geom2d::as<float>(konsole->get_mouseXY());
		}

		if (KeyReleased(_u('A')))
		{
			_INVERT_OPTION(Display);
		}

		if (KeyReleased(_u('M'))) {
			_INVERT_OPTION(Move);
		}

		if (KeyReleased(_u('L')))
		{
			_INVERT_OPTION(Palette);
		}

		////////////////////////////////////////// Movement //////////////////
#define ONLY_OPTION(name_option)			all_false(option, strMovementOptions);\
											option[#name_option].action = true;

		if (KeyReleased(_u('C'))) {
			ONLY_OPTION(Control);
			movement = &control;
		}

		if (KeyReleased(_u('F'))) {
			ONLY_OPTION(Free);
			movement = &freemove;
		}

		if (KeyReleased(_u('H'))) {
			ONLY_OPTION(Falldown);
			paramove.reset();
			paramove.set_functions(Xe, Ye);
			paramove.set_speed(0.003f);
			movement = &paramove;
		}

		if (KeyReleased(_u('P'))) {
			ONLY_OPTION(Parametric);
			paramove.reset();
			Seconds.reset();
			paramove.set_functions(X, Y);
			movement = &paramove;
		}
		
		if (KeyReleased(_u('J'))) {
			ONLY_OPTION(Jump);
			ground = sprite.get_position().y ;
			jump.set_ground(ground);
			movement = &jump;
		}
		if (option["Jump"].action)
		{
			Draw::hdraw(0, ground + sprite.get_dimension().y - 1,
				L'_', W, color::Red, background_color << 4);
		}


		if (KeyReleased(_u('T'))) {
			ONLY_OPTION(Move2points);
			move2p.set_2points( sprite.get_position(),mouse);
			movement = &move2p;
		}

		if (KeyReleased(_u('W'))) {
			move2points.reset();
			Seconds.reset();
			move2points.set_point_0(sprite.get_position());
			move2points.set_point_1(mouse);

			ONLY_OPTION(MoveAB);
			movement = &move2points;
		}

		if (option["MoveAB"].action)
		{
			move2points.set_point_0(sprite.get_position());
			move2points.set_point_1(mouse);
			move2points.reset();
		}

		if (KeyReleased(_u('Z'))) {
			ONLY_OPTION(Continus);
			movement = &continus;
		}

		////////////////////////////////////////// End Movement //////////////////

		if (KeyReleased(_u('N'))) {
			//
			str_box = make_box_string(3, 3, console::Hborder);
			sprite.set_string(str_box._str);
			sprite.set_size(str_box._lx, str_box._ly);
		}

		// Moving 
		if (option["Move"].action) {
			if (movement)
			movement->move(&sprite, ka_Dt);
		}
		 
		auto Ps = sprite.get_position().as<int>();
		std::wstring strPs(7, L' ');
		std::wstring cursor(7, L' ');
		swprintf_s(cursor.data(), 8, L"%3d|%2d", int(mouse.x), int(mouse.y));
		swprintf_s(strPs.data(), 7, L"%3d|%2d", Ps.x, Ps.y);
		

		// First Statu Bar
		TextUI::statu_bar(TextUI::StatuBarBuilder{}
			.set_position(0, H - level)
			.set_colorBG(color::GrayLight)
			.set_colorFG(color::Black)
			.set_colorChar(color::Red)
			.set_colorbg_string(color::YellowLight)
			.set_color_select(color::GrayDark)
			.set_step(1)
			.add_item(cursor	,W - 7	,false	,3)
			.add_item(L"Sprite[" + strPs + L"]", W - 22, false, 10)
		.get()
		);

		// Second Statu Bar
		TextUI::statu_bar(TextUI::StatuBarBuilder{}
			.set_position(0, H - level + 1)
			.set_colorBG(color::GrayLight)
			.set_colorFG(color::Black)
			.set_colorChar(color::Red)
			.set_colorbg_string(color::YellowLight)
			.set_color_select(color::GrayDark)
			.set_step(1)
			.add_item(L"A Display"	,0	, option["Display"].action	,0,2)
			.add_item(L"Move"		,0	, option["Move"].action			)
			.add_item(L"Palette"	,0	, option["Palette"].action		)
		.get()
		);

		// Third Statu Bar 
		// Time , Date, Mouse Coordinate, Option status.
		TextUI::statu_bar(TextUI::StatuBarBuilder{}
			.set_position(0, H - level + 2)
			.set_colorBG(color::Black)
			.set_colorFG(color::White)
			.set_colorChar(color::Red)
			.set_colorbg_string(color::YellowLight)
			.set_color_select(color::White)
			.set_color_selectFG(color::Black)
			.set_step(1)
			.add_item(L"Control"		,0	, option["Control"].action)
			.add_item(L"Jump"			,0	, option["Jump"].action)
			.add_item(L"Free"			,0	, option["Free"].action)
			.add_item(L"Parametric"		,0	, option["Parametric"].action)
			.add_item(L"H Fall"			,0	, option["Falldown"].action)
			.add_item(L"T Move2p"		,0	, option["Move2points"].action)
			.add_item(L"W FollowMouse"	,0	, option["MoveAB"].action)
			.add_item(L"Z ContinusMove"	,0	, option["Continus"].action)
		.get()
		);

		
		// draw objects
		sprite.draw();

		if (option["Palette"].action)
		{
			palette.draw();
			palette.select_color(color_box_fg,mouse.as<int>(), option["Palette"].action);
			sprite.color_fg(color_box_fg);
			sprite.color_bg(color_box_fg);
		}

		konsole->display();
	}





	return 0;
}
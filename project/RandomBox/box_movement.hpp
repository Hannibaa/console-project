/*				    Creation mode of box 
					Enum : Mode { Creation, Selection, ... }
						 : One Box creation methode
					TODO : full option box movement and creation methode.
*/
#pragma once
#include <vector>
#include <map>
#include <include/console/ka_utility.hpp>
#include <include/component/ka_ImageString.hpp>
#include <include/component/ka_SString.hpp>
#include <include/component/TextUI/ka_StatuBar.hpp>
#include <include/component/Physics/ka_Physics.hpp>
#include <include/component/ka_Draw.hpp>
#include <include/component/Physics/ka_FreeMove.hpp>
#include <include/component/Physics/ka_ControlMove.hpp>
#include <include/component/Physics/ka_Parametric_coord.hpp>
#include <include/component/Physics/ka_Jump.hpp>

#define _tow(val)			std::to_wstring(val)

using Sprite = wstringImg;
using vecSprite = std::vector<Sprite>;
using strButton = WSString;
using vecString = std::vector<std::wstring>;

using MapBool = std::map<std::string, bool>;

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

void all_false(MapBool& option) {
	for (auto& pair : option) pair.second = false;
}

class BoxBorder;
class BoxWBorder;


int game(int W, int H, int fw, int fh, int background_color = color::Blue) {

	int level = 3;
	konsole->construct_console(W, H, fw, fh);
	konsole->set_background_color(background_color << 4);

	konsole->set_drawWindows(0.f, 0.f, W, H - level);

	// set windows console static
	win::Set_ConsoleWindow_Style();
	// set window console position
	win::move_console_window(50, 50);

	const int Max_Option = 15;
	bool* option[Max_Option] = { false };

	// data here : 
	auto str_box = make_box_string(1, 1, console::Hborder);

	const int color_box_fg = background_color;
	const int color_box_bg = color::Red;

	Sprite  sprite(str_box._str,color_box_bg, color_box_fg);

	sprite.set_size(str_box._lx, str_box._ly);
	sprite.set_position(100.f, 10.f);
	
	// Bullet
	// /xfb7(6)=(B)
#define  _L  0x2588
#define  _LB 0x2501
	Sprite  bullet(std::wstring(2, _LB), color::Black, background_color);

	bullet.set_size(3, 1);
	bullet.set_position(60, 10);


	//////////////////////////////////////// Cinematics /////////////////////
	IMove* movement = nullptr;

	auto drawWindow = iRect{ 0,0 ,W, H - level }.as<float>();

	physic::ControlMove<Sprite> control;
	control.set_velocity(0.6f, 0.6f);
	control.set_space(drawWindow);

	physic::FreeMove0<Sprite> freemove;
	freemove.set_velocity(0.5f, 0.5f);
	freemove.set_space(drawWindow);

	auto X = [](float x) {float a = 70.f; float b = 15.f;
							return a + b * std::sinf(3.14f * 2.f * x); };

	auto Y = [](float x) {float a = 15.f; float b = 15.f;
							return a + b * std::cosf(3.14f * 2.f * x); };

	auto Xe = [&sprite](float x) { return sprite.get_position().x; };
	auto Ye = [&sprite](float x) {float g = 9.8f, v0 = 0.001f, y_ground = sprite.get_position().y;
		 return 0.5f * g * x * x - v0 * x + y_ground; };

	physic::ParaMove<Sprite> paramove(X,Y);
	paramove.set_speed(0.001);
	paramove.set_space(drawWindow);

	physic::Jump<Sprite>   jump(20.f, 0.7f);

	jump.set_velocity(0.6f, 0.7f);
	jump.set_space(drawWindow);

	physic::Move2p<Sprite>   move2p;

	move2p.set_space(drawWindow);
	move2p.set_velocity(0.01f, 0.01f);
	move2p.set_2points({ 1.f, 1.f }, { 100.f, 30.f });


	physic::PointMove<Sprite> move2points;

	move2points.set_period(1.0f);
	move2points.set_speed(0.01f);
	


	////////////////////////////////////////////////////////////////////////////
	// statu bars
	vecString vButtons = TextUI::strVectorBuilder{}
		.add(L"A Visual String")
		.add(L"S Selection")
		.add(L"C Control Move")
		.add(L"F Free Move")
		.add(L"N New Box")
		.add(L"J Jump Move")
		.add(L"P Param Move")
		.get();

	/// Number of option inferior of Max_Option
	int number_option = 8;

	bool option_visual_string = false;
	std::wstring _stat[2] = { L"Disable", L"Enable" };

	bool option_selection_enable = false; 

	/// Moving logic bool
	bool option_move = false;			  
	bool option_control = false;		  option[0] = &option_control;
	bool option_free = false;			  option[1] = &option_free;
	bool option_parametric = false;		  option[2] = &option_parametric;
	bool option_jump = false;			  option[3] = &option_jump;
	bool option_straight = false;		  option[4] = &option_straight;
	bool option_after_mouse = false;	  option[5] = &option_after_mouse;
	bool option_freefall = false;		  option[6] = &option_after_mouse;
	bool option_move2point = false;		  option[7] = &option_move2point;

	bool is_get_2point = false;

	bool option_2coord_points = false;  
	std::wstring _str_2coord[2] = { L"Coord Disable", L"Get 2 Coordinate Points" };
	fVec2  pt1{}, pt2{};
	bool _b2points = false;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//   Main Loop    
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	Time::Game_Timer timer;
	fVec2			 mouse;

	while (konsole->is_open()) {

		konsole->clear();

		// primitive parameter of konsole
		KA_MAKE_FPS(timer);
		KA_MAKE_DALTA_TIME(1.f);

		PRINT_TITLE(L"Random Box fps[%4.2f][%1.4f]", ka_fps, ka_Dt);

		while (konsole->pull_event()) {
			mouse = geom2d::as<float>(konsole->get_mouseXY());
		}

		if (KeyReleased(_u('A')))
		{
			// implemetation for A option
			option_visual_string = !option_visual_string;
		}

		if (KeyReleased(_u('M'))) {
			// implementation for D option
			option_move = !option_move;
		}

		if (KeyReleased(_u('C'))) {
			all_false(option, number_option);
			option_control = true;

			movement = &control;
		}

		if (KeyReleased(_u('F'))) {
			all_false(option, number_option);
			option_free = true;

			movement = &freemove;
		}

		if (KeyReleased(_u('P'))) {
			all_false(option, number_option);
			option_parametric = true;
			paramove.reset();
			paramove.set_functions(X, Y);
			movement = &paramove;
		}

		if (KeyReleased(_u('S'))) {
			// implementation for S
			option_selection_enable = !option_selection_enable;
		}

		if (KeyReleased(_u('N'))) {
			//
			str_box = make_box_string(3, 3, console::Hborder);
			sprite.set_string(str_box._str);
			sprite.set_size(str_box._lx, str_box._ly);
		}

		if (KeyReleased(_u('J'))) {
			all_false(option, number_option);
			option_jump = true;

			movement = &jump;
		}

		if (KeyReleased(_u('G'))) {
			// get two coordinate points
			option_2coord_points = !option_2coord_points;
		}

		if (KeyReleased(_u('T'))) {
			// straight between two point pt1 to pt2
			all_false(option, number_option);

			option_straight = true;
			movement = &move2p;
		}

		if (KeyReleased(_u('W'))) {
			all_false(option, number_option);
			move2points.reset();
			move2points.set_point_0(sprite.get_position());
			option_after_mouse = true;
		}

		if (KeyReleased(_u('V'))) {
			if (option_straight)
			move2p.set_2points(pt1, pt2);
		}

		if (KeyReleased(_u('H'))) {
			all_false(option, number_option);
			option_freefall = true;
			paramove.reset();
			paramove.set_functions(Xe, Ye);
			movement = &paramove;
		}


		if (option_after_mouse)
		{
			if (KeyReleased(VK_LBUTTON)) {
				pt2 = mouse;
				move2points.set_point_1(pt2);
				is_get_2point = true;
			}

			if( is_get_2point )
			movement = &move2points;

			if ((sprite.get_position() - pt2).Norm() < 2.5)
			{
				move2points.set_point_0(pt2);
				is_get_2point = false;
				move2points.reset();
			}
		}
		
		if (option_2coord_points) {
			if (KeyPressed(VK_CONTROL))
				if (KeyReleased(VK_LBUTTON))
				{
					if (!_b2points)
					{
						pt1 = mouse;
						_b2points = true;
					}
					else
					{
						pt2 = mouse;
						option_2coord_points = false;
						_b2points = false;
					}
				}
		}

		if (option_visual_string) {
			Draw::draw_str(10, 10, str_box._str, color::Red, color::Black);
			Draw::draw_str(0, 0,L"pt1 : " + _tow(int(pt1.x)) + L"|" + _tow(int(pt1.y)),
				color::White, background_color);
			Draw::draw_str(0, 1,L"pt2 : " + _tow(int(pt2.x)) + L"|" + _tow(int(pt2.y)),
				color::White, background_color);
		}

		if (option_selection_enable) {

			if (sprite.get_bounds().contain(mouse))
			{
				sprite.color_bg(color::YellowLight);

				if (KeyPressed(VK_LBUTTON))
				{
					sprite.set_position(mouse);
				}
			}
			else
				sprite.color_bg(color_box_bg);

		}

		// Moving 
		if (option_move) {
			if (movement)
			movement->move(&sprite, ka_Dt);
		}

		if (option_jump)
		{
			Draw::hdraw(0, jump.get_ground() + sprite.get_dimension().y - 1,
				L'_', W, color::Red, background_color << 4);
		}
		
		 
		auto Ps = sprite.get_position().as<int>();

		TextUI::StatuBar(vButtons);

		TextUI::StatuBar(TextUI::StatuBarBuilder{}
			.set_position(0,H - level + 1)
			.set_colorBG(color::GrayDark)
			.set_colorFG(color::Black)
			.set_colorbg_string(color::GrayLight)
			.set_colorChar(color::Red)
			.set_step(1)
			.add_item(L"G Get 2Points")
			.add_item(L"T Move 2Points")
			.add_item(L"V Select New 2Points")
			.add_item(L"H Free Fall")
			.add_item(L"W Mouse 2Point")
			.add_item(L"B Bullet")
			.get()
		);

		TextUI::StatuBar(TextUI::StatuBarBuilder{}
			.set_position(0, H - level)
			.set_colorBG(color::Yellow)
			.set_colorFG(color::White)
			.set_colorChar(color::Red)
			.set_colorbg_string(color::BlueLight)
			.set_step(2)
			.add_item(L"Esc Close")
			.add_item(L"Visual[" + _stat[option_visual_string] + L"]")
			.add_item(L"Select[" + _stat[option_selection_enable] + L"]")
			.add_item(L"Move[" + _stat[option_move] + L"]")
			.add_item(L"Control[" + _stat[option_control] + L"]")
			.add_item(L"Free[" + _stat[option_free] + L"]")
			.add_item(L"Para[" + _stat[option_parametric] + L"]")
			.add_item(L"Jump[" + _stat[option_jump] + L"]")
			.add_item(L"X[" + _tow(Ps.x) + L"]")
			.add_item(L"Y[" + _tow(Ps.y) + L"]")
			.add_item(_str_2coord[option_2coord_points])
			.get());

		// draw objects
		sprite.draw();
		bullet.draw();

		konsole->display();
	}





	return 0;
}
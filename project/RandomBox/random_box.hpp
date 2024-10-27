#pragma once
#include <vector>
#include <include/console/ka_utility.hpp>
#include <include/component/ka_ImageString.hpp>
#include <include/component/ka_SString.hpp>
#include <include/component/Physics/ka_Kinematics.h>
#include <include/component/TextUI/ka_StatuBar.hpp>
#include <include/component/Physics/ka_Physics.hpp>

#define _tow(val)			std::to_wstring(val)

using Sprite = wstringImg;
using vecSprite = std::vector<Sprite>;
using strButton = WSString;
using vecString = std::vector<std::wstring>;

class ControlMove : public physic::IMove<Sprite, fVec2, fRect>
{
public:
	void move(Sprite* ptr, float Dt) override {

		auto p = ptr->get_position();
		auto d = ptr->get_dimension();
		
		if (KeyPressed(VK_UP)) {
			p.y -= _velocity.y * Dt;
		}

		if (KeyPressed(VK_LEFT)) {
			p.x -= _velocity.x * Dt;
		}

		if (KeyPressed(VK_RIGHT)) {
			p.x += _velocity.x * Dt;
		}

		if (KeyPressed(VK_DOWN)) {
			p.y += _velocity.y * Dt;
		}

		ptr->set_position(p);
	}
};

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

Sprite make_random_box( int W, int H)
{
	Sprite sp;
	rng::RG<int> l_x(0, W);
	rng::RG<int> l_y(0, H);

	int lx = l_x();
	int ly = l_y();

	sp.set_string(make_box_string(lx, ly, console::Hborder));
	sp.set_size(lx, ly);
	sp.set_position(l_x(), l_y());
	sp.color_bg(konsole->get_background_color()>>4);
	sp.color_fg(color::Red);

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

int game(int W, int H, int fw, int fh, int background_color = color::Blue) {


	konsole->construct_console(W, H, fw, fh);
	konsole->set_background_color(background_color << 4);
	win::Set_ConsoleWindow_Style();
	win::move_console_window(50, 50);
	

	// 1. Data
	int lx{6};
	int ly{3};
	fVec2 p0(40.f, 10.f);
	bool b_On;
	bool b_select{ false };
	bool b_collision{ false };

	std::wstring Mode{};
	std::wstring Mode_collision{};
	
	
	vecSprite  vec;
	// statu bars
	vecString vButtons = TextUI::strVectorBuilder{}
		.add(L"A Add Random box")
		.add(L"D add box")
		.add(L"S Selection")
		.add(L"C Collision")
	.get();
	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//   Main Loop    
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	Time::Game_Timer timer;
	fVec2			 mouse;
	iVec2			 p1{};
	iVec2			 p2{};

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
			vec.push_back(make_random_box(W,H));
		}

		if (KeyReleased(_u('D'))) {
			if (p1 != p2) {
				auto p = p1 - iVec2(1, 1);
				vec.push_back(make_box(p.as<float>(), p2.as<float>(), color::White));
				p1 = p2;
			}
		}

		if (KeyReleased(_u('C'))) {
			b_collision = !b_collision;
		}

		if (KeyReleased(_u('S'))) {
			b_select = !b_select;
		}

		// TODO selection of boxes
		if (b_select) {
			Mode = L"SELECTION";
			for (auto it = vec.begin(); it != vec.end(); ++it) {
				if (it->get_bounds().contain(mouse))
				{
					it->color_bg(color::GrayDark);
					//break;
				}
				else
					it->color_bg(background_color);
			}
		}

		if (!b_select) {
			Mode = L"CREATION";
			get_2points(p1, p2, b_On, mouse, VK_LBUTTON);
		}

		Draw::border_box(console::Hborder, p1, p2, color::White, background_color);

		// add collision
		// TODO : NEED TO UPDATE get_bounds() functions to get right rectangle
		if (b_collision) {
			Mode_collision = L"COLLISION";
			physic::collision_process(vec);
		}
		else
		{
			Mode_collision = L"OVERLAPS";
		}

		// drawing ...
		TextUI::StatuBar(vButtons);
		TextUI::StatuBar(TextUI::StatuBarBuilder{}
			.set_position(0, H - 2)
			.set_colorBG(color::Yellow << 4)
			.set_colorFG(color::White)
			.set_colorChar(color::Red)
			.set_step(2)
			.add_item(L"P1 : " + _tow(p1.x) + L"|" + _tow(p1.y))
			.add_item(L"P2 : " + _tow(p2.x) + L"|" + _tow(p2.y))
			.add_item(L"Size vec " + _tow(vec.size()))
			.add_item(L"Mode[" + Mode + L"]")
			.add_item(L"Collision[ " + Mode_collision + L" ]")
			.get());

		for (const auto& s : vec) s.draw();

		konsole->display();
	}














	return 0;
}
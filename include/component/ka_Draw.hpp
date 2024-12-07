#pragma once
#include <include/console/ka_utility.hpp>






namespace Draw {

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//			DRAWING METHODE   
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Drawing string at coordinate x,y with color fg and bg :
	void draw_str(int x, int y, const std::wstring& wstr, int colorfg, int colorbg) {
		int i{};
		for (const auto& c : wstr)
			konsole->draw(x + (++i), y, c, colorfg | colorbg << 4);
	}

	void draw_strh(int x, int y, const std::wstring& wstr, int colorfg, int colorbg) {
		int i{};
		for (const auto& c : wstr)
			konsole->draw(x, y + (++i), c, colorfg | colorbg << 4);
	}

	void hdraw(int x, int y, wchar_t c, int rep, short colorfg, short colorbg = console::BG_BLACK)
	{
		for (int i = x; i < x + rep; ++i)
			konsole->draw(i, y, c, colorfg | colorbg);
	}

	void vdraw(int x, int y, wchar_t c, int rep, short colorfg, short colorbg = console::BG_BLACK)
	{
		for (int i = y; i < y + rep; ++i)
			konsole->draw(x, i, c, colorfg | colorbg);
	}








//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//		Box Drawing    
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Drawing methode defined here
	void box(int x, int y, int lx, int ly, int colorfg, int colorbg = -1)
	{
		if (colorbg == -1) colorbg = konsole->get_background_color();

		for (int i = x; i < x + lx; ++i)
			for (int j = y; j < y + ly; ++j)
			{
				konsole->draw(i, j, L' ', colorfg | colorbg);
			}

	}

	void box(const iRect& rect, int _colorfg, int _colorbg = -1) {
		box(rect.x, rect.y, rect.dx, rect.dy, _colorfg, _colorbg);
	}


	void shadow(const iRect& box, int color_shader)
	{
		for (int i = box.x + 1; i < box.x + 1 + box.dx; ++i)
			konsole->draw(i, box.y + box.dy, L' ', color_shader);

		for (int j = box.y + 1; j < box.y + 1 + box.dy; ++j)
			konsole->draw(box.x + box.dx, j, L' ', color_shader);
	}



	template<typename TRect, typename T = typename TRect::value_type>
		requires std::is_arithmetic_v<T> || std::is_enum_v<T> &&
	std::is_arithmetic_v<typename TRect::value_type>
		void box(const TRect & rect, int _colorfg, int _colorbg = -1) {
		box(
			static_cast<int>(rect.x),
			static_cast<int>(rect.y),
			static_cast<int>(rect.dx),
			static_cast<int>(rect.dy), _colorfg, _colorbg);
	}

	// with border

	void border_box(const wchar_t (&_border)[6], int x, int y, int dx, int dy, int colorfg, int colorbg)
	{
		colorbg = colorbg << 4;
		// Corner
		konsole->draw(x - 1, y - 1, _border[0], colorfg | colorbg);
		konsole->draw(x + dx - 1, y - 1, _border[2], colorfg | colorbg);
		konsole->draw(x + dx - 1, y + dy - 1, _border[4], colorfg | colorbg);
		konsole->draw(x - 1, y + dy - 1, _border[5], colorfg | colorbg);
		//// Lines
		//int rep = dx - _title.size() - 3;
		Draw::hdraw(x, y - 1, _border[1], dx - 1, colorfg, colorbg);
		Draw::vdraw(x - 1, y, _border[3], dy - 1, colorfg, colorbg);
		Draw::hdraw(x, y + dy - 1, _border[1], dx - 1, colorfg, colorbg);
		Draw::vdraw(x + dx - 1, y, _border[3], dy - 1, colorfg, colorbg);
	}

	void border_box(const wchar_t(&_border)[6], const iVec2& p1, const iVec2& p2, int colorfg, int colorbg)
	{
		border_box(_border, p1.x, p1.y, p2.x - p1.x, p2.y - p1.y, colorfg, colorbg);
	}

} // End of namespace Draw
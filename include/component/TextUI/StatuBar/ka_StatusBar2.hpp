/*
			Statu Bars Design
			Author : Kadda Aoues
			Date	: 11 / 30 / 2024
			Version	: 1.0.0

*/

#pragma once
#include <console project/include/component/ka_Draw.hpp>
#include "ka_ItemSB.hpp"


namespace TextUI {

	using vItems = std::vector<SBItem>;
	
	struct sStatuBar2 {
		int x;
		int y;

		int step;
		int fstep;

		int color_fg;
		int color_bg;
		int color_sel;
		int color_char;
		int color_string_bg;

		vItems vitems;
	};

	void statu_bar(const sStatuBar2& sb) {
		int l(sb.fstep);
		int width = konsole->width();

		Draw::box(sb.x, sb.y, width, 1, sb.color_fg, sb.color_bg << 4);

		for (const auto& item : sb.vitems)
		{
			if (item._pos != 0)
				item.draw(sb.x + item._pos, sb.y);
			else
			{
				item.draw(sb.x + l, sb.y);
				l += item._str.size() + sb.step;
			}

			if (l > width) break;
		}
	}

	class StatuBarBuilder {

		sStatuBar2  _statuBar;

	public:

		StatuBarBuilder& set_position(int x, int y) {
			_statuBar.x = x;
			_statuBar.y = y;

			return *this;
		}

		StatuBarBuilder& set_step(int _step, int _firstStep = 0) {
			_statuBar.step = _step;
			_statuBar.fstep = _firstStep;

			return *this;
		}

		StatuBarBuilder& set_colorFG(int col) {
			_statuBar.color_fg = col;
			return *this;
		}

		StatuBarBuilder& set_colorBG(int col) {
			_statuBar.color_bg = col;
			return *this;
		}

		StatuBarBuilder& set_color_select(int col) {
			_statuBar.color_sel = col;
			return *this;
		}


		StatuBarBuilder& add_item(const SBItem& item) {
			_statuBar.vitems.push_back(item);

			return *this;
		}

		StatuBarBuilder& set_colorChar(int col) {
			_statuBar.color_char = col;
			return *this;
		}

		StatuBarBuilder& set_colorbg_string(int col) {
			_statuBar.color_string_bg = col;
			return *this;
		}

		StatuBarBuilder& add_item(
			const std::wstring text ,// Text to be displayed in statu bar
			int position = 0,		 // at position in statu bar
			bool _b = false,		 // 0 bg color, 1 bg color of selection
			int posChar = 0, 		 // position of first char to colored
			int length = 1 			 // length of chars to be colored
		) {
			SBItem item;
			int col[2] = { _statuBar.color_bg,_statuBar.color_sel };
			item._str = text;
			item._color_bg = col[_b];
			item._color_fg = _statuBar.color_fg;
			item._posChar = posChar;
			item._lenChar = length;
			item._pos = position;
			item._colorChar = _statuBar.color_char;
			
			_statuBar.vitems.push_back(item);
			return *this;
		}

		StatuBarBuilder& set_colorBG(int posItem, int colorbg)
		{
			if (posItem < _statuBar.vitems.size())
			_statuBar.vitems[posItem]._color_bg = colorbg;
			return *this;
		}

		StatuBarBuilder& set_ItemPosition(int posItem, int _position) {
			if (posItem < _statuBar.vitems.size())
				_statuBar.vitems[posItem]._pos = _position;
			return *this;
		}

		sStatuBar2 get() const {
			return std::move(_statuBar);
		}

	};
}
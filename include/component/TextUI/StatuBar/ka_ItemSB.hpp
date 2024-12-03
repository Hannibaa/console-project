//					this class related to statubar only
//					Autor : Kadda Aoues

#pragma once
#include <console project/include/console/ka_utility.hpp>




namespace TextUI{

	struct SBItem {
		std::wstring		_str;			// Item to be viewed
											// 
		int					_color_bg;		// initial background color
		int					_color_fg;		// foreground color for all char
		int					_pos{0};			// position
		int					_posChar;		// position of special char to be colored
		int					_lenChar;		// length of special char to be colored
		int					_colorChar;		// foreground of special chars
		
		void draw(int x,int y) const {
			for (int i = 0; i < _str.size(); ++i)
				konsole->draw(x + i, y, _str[i], _color_fg | _color_bg << 4);

			for (int i = _posChar; i < _posChar + _lenChar; ++i)
				if (i < _str.size())
				konsole->draw(x + i, y, _str[i], _colorChar | _color_bg << 4);
		}
	};



	class ItemBuilder {
		SBItem   item;

	public:

		ItemBuilder& set_string(const std::wstring& text) {
			item._str = text;
			return *this;
		}

		ItemBuilder& set_colorBG(int color){ 
			item._color_bg = color;
			return *this;
		}
		ItemBuilder& set_colorFG(int color){ 
			item._color_fg = color;
			return *this;
		}

		ItemBuilder& set_position(int _position) {
			item._pos = _position;
			return *this;
		}

		ItemBuilder& set_ParaChar(int pos, int length, int colorChar){ 
			item._posChar = pos;
			item._lenChar = length;
			item._colorChar = colorChar;
			return *this;
		}
		

		SBItem get() const {
			return std::move(item);
		}
	};

}
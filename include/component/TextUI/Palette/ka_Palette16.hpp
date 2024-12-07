/*
					Palette UI to chose color in console
					Author		:	Kadda Aoues
					Date		:	6 / 12 / 2024
					Discription	:	this will return a color from palette

					TODO :
					-------
					1. need to fix array of rectangle contain or rect function border.
*/
#pragma once
#include <console project/include/component/ka_Draw.hpp>
#include <include/Macros.hpp>
#include <console project/include/basic/ka_IDrawable.hpp>
#include <console project/include/basic/ka_ITransformable.hpp>
#include <console project/include/console/ka_color.hpp>


namespace TextUI {

	class Palette16 : public console::IDrawable, public console::ITransformable<int>
	{
		const int		_plx;
		const int		_ply;
		const int		_ncolors;
		int				_ncolmns;

		iRect					_rect;

		std::array<iRect, 16>	_colRect;

		void make_color_rect() {
			for (int col = 0; col < 16; ++col)
			{
				_colRect[col].x = this->get_position().x + 1;
				_colRect[col].y = this->get_position().y + col;
				_colRect[col].dx = _plx ;
				_colRect[col].dy = 1;
			}

			_rect.x = this->get_position().x;
			_rect.y = this->get_position().y;
			_rect.dx = _plx + 1;
			_rect.dy = _ply;
		}

	public:

		Palette16(int x, int y)
			:_plx(6)
			,_ply(16)
			,_ncolors(16)
			,_ncolmns(1)
		{
			this->set_position(x, y);
			make_color_rect();

		}



		virtual vector_type get_dimension() const override {
			return iVec2(_plx, _ply);
		}

		virtual rect_type	get_bounds() const override {
			iRect rect;
			rect.x = this->get_position().x;
			rect.y = this->get_position().y;
			rect.dx = _plx;
			rect.dy = _ply;
			return rect;
		}

		int select_color2(const iVec2& mouse, bool& bCancel)
		{
			
			if (_rect.contain(mouse))
			{
				if (KeyPressed(VK_LBUTTON))
				{
					int count{};
					for (const auto& rect : _colRect)
					{
						if (rect.contain(mouse))
						{
							bCancel = false;
							return count;
						}
						++count;
					}
				}
			}

			return -1;
		}

		void select_color(int& col, const iVec2& mouse, bool& bCancel)
		{
			if (_rect.contain(mouse))
			{
				if (KeyPressed(VK_LBUTTON))
				{
					int count{};
					for (const auto& rect : _colRect)
					{
						if (rect.contain(mouse))
						{
							bCancel = false;
							col = count;
						}
						++count;
					}
				}
			}
		}

		virtual void draw() const override {
			iVec2 pos = this->get_position();
			int lcolumn = _plx / _ncolmns;

			for (int p = 0; p < _ncolors; ++p)
			{
				Draw::draw_str(pos.x, pos.y + p, std::wstring(lcolumn, L' '), color::Black, p);
			}
		}

	};

}
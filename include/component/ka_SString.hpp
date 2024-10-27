//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//     String drawable and transformable inherite from standar string.
//     Author  : Kadda Aoues
//     Date	   : 10 / 15 /2024
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include <include/console/ka_utility.hpp>
#include <include/basic/ka_IDrawable.hpp>
#include <include/basic/ka_ITransformable.hpp>



namespace console {

	template<typename Tchar>
	class SString : public std::basic_string<Tchar>, public IDrawable, public ITransformable<float>
	{
		int				_colorbg;
		int				_colorfg;
		int				_Color0BG;
		int				_Color0FG;

	public:
		using std::basic_string<Tchar>::basic_string;

		SString(const std::basic_string<Tchar>& image, iVec2 colors)
		{
			this->assign(image);

		   _colorfg = colors.x;
		   _colorbg = colors.y;
		   _Color0BG = _colorbg;
		   _Color0FG = _colorfg;
		}

		std::pair<int, int> get_originalColor() const {
			return std::pair<int, int>(_Color0FG, _Color0BG);
		}

		void set_colorFG(int col) {
			_colorfg = col;
		}
		
		void set_colorBG(int col) {
			_colorbg = col;
		}

		int get_colorFG() const {
			return _colorfg;
		}

		int get_colorBG() const {
			return _colorbg;
		}

		fVec2 get_dimension() const {
			return fVec2{ float(this->size()), 1.f };
		}

		fRect get_bounds() const {
			auto p = get_position();
			auto s = get_dimension();
			return fRect{ p.x, p.y, s.x, s.y };
		}

		void draw() const override {
			iVec2 _position = this->get_position().as<int>();
			int i{};
			for (const auto& c : *this)
				konsole->draw(_position.x + i++, _position.y, c, _colorfg | (_colorbg << 4));
		}

	};



}

using WSString = console::SString<wchar_t>;
using SString = console::SString<char>;
#pragma once
#include <iostream>
#include <include/console/ka_utility.hpp>
#include <include/basic/ka_IColorString.hpp>
#include <include/basic/ka_ITransformable.hpp>
#include <include/random/random_generator.h>
#include <include/maths/clamp.hpp>



namespace console {

	template<typename Tchar>
	class ImageString : public IColorString<Tchar>, public ITransformable<float>, public IDrawable
	{
		int						_lx;
		int						_ly;

	public:
		using IColorString<Tchar>::IColorString;
		using IColorString<Tchar>::operator[];
		using IColorString<Tchar>::size;
		using IColorString<Tchar>::resize;

		ImageString& operator = (const ImageString& other)
		{
			this->_lx = other._lx;
			this->_ly = other._ly;
			this->set_position(other.get_position());
			this->set_stringAndColor(other.string(), other.get_attributes());

			return *this;
		}

		ImageString(wchar_t w, int lx, int ly)
			:IColorString<Tchar>(std::wstring(lx * ly, w))
		{
			_lx = lx;
			_ly = ly;
		}

		void set_size(int lx, int ly) {
			_lx = lx;
			_ly = ly;
		}


		virtual void draw() const override {
			auto p = this->get_position();
			auto dim = this->get_dimension();
			auto drect = konsole->get_drawWindows();

			math::clamp(p.x, drect.x, drect.x + drect.dx - dim.x);
			math::clamp(p.y, drect.y, drect.y + drect.dy - dim.y);

			for (int i = 0; i < _lx; ++i)
				for (int j = 0; j < _ly; ++j)
				konsole->draw(p.x + i, p.y + j, 
					this->operator[](j*_lx + i).Char, this->operator[](j*_lx + i).Attributes);
		}

		fVec2 get_dimension() const override {
			return fVec2{ float(_lx), float(_ly)};
		}

		fRect get_bounds() const {
			fVec2 p = this->get_position();
			return fRect(p.x, p.y, _lx, _ly);
		}
	};


	// stream operator << 
	template<typename TChar>
	std::basic_ostream<TChar>& operator << (std::basic_ostream<TChar>& os,
		const ImageString<TChar>& str)
	{
		os << str.string();
		return os;
	}

}

using wstringImg = console::ImageString<wchar_t>;
using stringImg = console::ImageString<wchar_t>;


namespace strImg {

	class WStringImageBuilder {
		wstringImg _strImg;
	public:

		WStringImageBuilder& set_string(const std::wstring& str) {
			_strImg.set_string(str);
			return *this;
		}
		WStringImageBuilder& set_position(float x, float y) {
			_strImg.set_position(x, y);
			return *this;
		}
		WStringImageBuilder& set_colorbg(int _color) {
			_strImg.color_bg(_color);
			return *this;
		}
		WStringImageBuilder& set_colorfg(int _color) {
			_strImg.color_fg(_color);
			return *this;
		}
		WStringImageBuilder& set_size(int lx, int ly) {
			_strImg.set_size(lx, ly);
			return *this;
		}

		wstringImg get() const {
			return std::move(_strImg);
		}

	};


	wstringImg make_StringImage(
		const std::wstring& str, 
		int lx, int ly, 
		int colorbg ,
		float minx, float maxx,
		float miny, float maxy)
	{
		rng::fRG<float>  frand;
		float x = frand(minx, maxx);
		float y = frand(miny,maxy);

		return WStringImageBuilder{}
			.set_string(str)
			.set_position(x, y)
			.set_colorbg(colorbg)
			.set_colorfg(color::White)
			.set_size(lx, ly)
			.get();
	}

	// make random square
	wstringImg make_StringImage(const std::wstring& str, int lx, int ly, int colorbg = color::Black)
	{

		rng::fRG<float>  frand;
		float x = frand(1.f, 140.f);
		float y = frand(1.f, 30.f);

		return WStringImageBuilder{}
			.set_string(str)
			.set_position(x, y)
			.set_colorbg(colorbg)
			.set_colorfg(color::White)
			.set_size(lx, ly)
			.get();
	}

	// colored boxed lx ly
	wstringImg make_StringImage(int lx, int ly) {
		return make_StringImage(std::wstring(lx * ly, L' '), lx, ly);
	}

	////// colored boxes lx ly and color
	wstringImg make_StringImage(int lx, int ly, int col) {
		return make_StringImage(std::wstring(lx * ly, L' '), lx, ly, col);
	}

	// only alphabet
	std::vector<wstringImg> make_spliteString(const std::wstring wstr) {

		std::vector<wstringImg> vStr;
		for (const auto& c : wstr)
		{
			vStr.push_back(make_StringImage(std::wstring(1, c), 1, 1));
		}

		return vStr;

	}

}

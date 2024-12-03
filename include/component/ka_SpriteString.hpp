#pragma once
#include <functional>
#include <iostream>
#include <include/console/ka_utility.hpp>
#include <include/basic/ka_IColorString.hpp>
#include <include/basic/ka_ITransformable.hpp>
#include <include/maths/clamp.hpp>



namespace console {


	template<typename Tchar>
	class SpriteString : public IColorString<Tchar>, public ITransformable<float>, public IDrawable
	{
	public:
		using value_type = Tchar;
		using Function = std::function<bool(value_type)>;

	private:
		int				_id;
		int				_lx;
		int				_ly;
		Function		_function{ [](Tchar x)->bool {return false; } };   // to check is it char or not

	public:
		using IColorString<Tchar>::IColorString;
		using IColorString<Tchar>::operator[];
		using IColorString<Tchar>::size;
		using IColorString<Tchar>::resize;


		SpriteString& operator = (const SpriteString& other)
		{
			this->_lx = other._lx;
			this->_ly = other._ly;
			this->set_position(other.get_position());
			this->set_stringAndColor(other.string(), other.get_attributes());

			return *this;
		}

		SpriteString(wchar_t w, int lx, int ly)
			:IColorString<Tchar>(std::wstring(lx * ly, w))
		{
			_lx = lx;
			_ly = ly;
		}

		void set_id(int id) {
			_id = id;
		}

		int get_id() const {
			return _id;
		}

		void set_function(std::function<bool(Tchar)>   func)
		{
			_function = std::move(func);
		}

		void set_size(int lx, int ly) {
			if (this->size() < 2) {
				_lx = 1; _ly = 1;
			}
			else if (this->size() < lx * ly) {
				_lx = this->size();
				_ly = 1;
			}
			else
			{
				_lx = lx;
				_ly = ly;
			}

		}

		virtual void draw() const override {
			auto p = this->get_position();
			auto dim = this->get_dimension();
			auto drect = konsole->get_drawWindows();

			math::clamp(p.x, drect.x, drect.x + drect.dx - dim.x);
			math::clamp(p.y, drect.y, drect.y + drect.dy - dim.y);

			for (int i = 0; i < _lx; ++i)
				for (int j = 0; j < _ly; ++j)
				{
					if (! _function(this->operator[](j* _lx + i).Char))
					konsole->draw(p.x + i, p.y + j,
						this->operator[](j* _lx + i).Char, this->operator[](j* _lx + i).Attributes);
				}
		}

		fVec2 get_dimension() const override {
			return fVec2{ float(_lx), float(_ly) };
		}

		fRect get_bounds() const {
			fVec2 p = this->get_position();
			return fRect(p.x , p.y , _lx, _ly);
		}
	};


}
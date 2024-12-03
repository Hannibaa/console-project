#pragma once
#include <console project/include/console/ka_utility.hpp>








namespace physic {


	template<size_t Capacity, typename ...Tshapes>
	class ShapesManager 
	{
		using Shapes = std::variant<Tshapes...>;

		std::vector<Shapes>  _shapes;
		Shapes* _ptr;

	public:
		using value_type = Shapes;
		using vector_type = std::vector;

		ShapesManager()
			:_ptr(nullptr)
		{
			_shapes.reserve(Capacity);
		}

		void add(const Shapes& s) {
			if (_shapes.size() < Capacity)
				_shapes.push_back(s);
			else
				std::cerr << "Error : Should be not exceed capacity of : " << Capacity << '\n';
		}

		size_t size() const {
			return _shapes.size();
		}

		void remove() {

		}

		Shapes& at(size_t i) {
			if (i < _shapes.size())
				return _shapes.at(i);
		}

		Shapes at(size_t i) const {
			if (i < _shapes.size())
				return _shapes.at(i);
		}

		bool IsCollid(const Shapes& s1, const Shapes& s2) {
			return std::visit([](const auto& a, const auto& b)->bool {
				return physic::is_collid(a, b);
				}, s1, s2);
		}

		void manage_collision(float r = 2.0f) { //<decltype(s1), decltype(s2)>

			for (auto& s1 : _shapes)
				for (auto& s2 : _shapes)
				{
					if (IsCollid(s1, s2))
					{
						std::visit([r](auto& s1, auto& s2)->void {
							physic::collision_process(s1, s2, r);
							}, s1, s2);
						break;
					}
				}
		}

		Shapes* select_item(const fVec2& mouse) { //s
			_ptr = nullptr;
			for (auto& s : _shapes) {
				if (std::visit([=](const auto& ss)->bool {return ss.get_bounds().contain(mouse); }, s))
				{
					_ptr = &s;
					break;
				}
			}

			return _ptr;
		}

		void draw() const {
			for (auto& s : _shapes)
				std::visit([](const auto& s) {
				s.draw();
					}, s);
		}
	};




}


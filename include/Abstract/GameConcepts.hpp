/*				THIS HEADER FOR GAME OBJECT AND TYPE CONCEPTS AND METAPROGRAMMING 
				Author  : Kadda Aoues 
				Date	: 18 / 10 / 2024
*/

#pragma once
#include <concepts>

// 
namespace Concept {

	// check if object have same function 
#define REQUERES(_Class, _Type ,_Function)  requires(_Class C,_Type value){ C._Function(value);}
#define MAKE_HAS_FUNCTION(_Class, _Type, _Function) template<typename _Class, typename _Type> \
          concept  Has_##_Function = REQUERES(_Class, _Type, _Function)                       \

	namespace Coordinate {
		// 0. hase composante x, y ro X, Y
		template<typename Coord>
		concept has_XY = requires(Coord coord)
		{
			coord.x;
			coord.y;
		};

		template<typename Coord>
		concept has_xy = requires(Coord coord)
		{
			coord.X;
			coord.Y;
		};
	}
	// 1. check if class has x and y variable or X, y
	template<typename V>
	concept is_geometrical_vector = requires(V v)
	{
		typename V::value_type;
		requires std::is_arithmetic_v<typename V::value_type>;
		v.x;
		v.y;
		v + v;
		v - v;
		{ (v * v) } -> std::convertible_to<typename V::value_type>;
	};

	// 2.



} // End Of Concept namespace
/*										Utility Console								*/
#pragma once


#include <iostream>
#include <vector>
#include <sstream>
#include <cwctype>
#include <tuple>
#include <array>
#include <utility>
#include <include/console/ka_color.hpp>
#include <include/time/Game_timer.h>
#include <include/console/ka_console.hpp>
#include <include/geometry/vector2d.hpp>
#include <include/geometry/rectangle.hpp>
#include <include/metaprogramming/my_metaprog.h>



#define MAX_CHAR_TITLE				  256
#define PRINT_TITLE(format, ... )     swprintf_s(console::title, MAX_CHAR_TITLE - 1, format , __VA_ARGS__); \
                                      TITLE(console::title)


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    make unique pointer to console as globale variable
//	  Konsole as unique pointer to object Console
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

using Konsole = std::unique_ptr<console::Console>;

Konsole  konsole = std::make_unique<console::Console>();


namespace console {

	// 
	// We will make also globle variable Need it for game programming
	// 
	float				 fps{ 1.f };					// frame per seconds = 1.f / elaps
	float				 elaps{};						// elapsed time
	float				 Dt60{ 1.5f };					// dalta time in 60 seconds - steps time -
	float				 Dt{};						    // Dt = Dt60 * 60.f / fps;
	wchar_t				 title[MAX_CHAR_TITLE]{};       // buffer for title 

#define		 ka_fps      console::fps
#define		 ka_elaps	 console::elaps
#define		 ka_Dt60	 console::Dt60
#define		 ka_Dt  	 console::Dt

	// Define Macro to calculate fps and elaps timer without repeating code every time
#define KA_FPS(timer)     				ka_elaps = timer##.get_elapsed_time();    \
										timer##.reset();						  \
										ka_fps = 1.f / ka_elaps;

#define KA_MAKE_FPS(timer)				KA_FPS(timer)

#define KA_MAKE_DALTA_TIME(Float)		ka_Dt60		  = float(Float)	;		  \
										ka_Dt		  = ka_Dt60 * 60.f * ka_elaps; 

	const wchar_t Lborder[6] = { 0x250c ,0x2500,0x2510, 0x2502 ,0x2518,0x2514 }; // light border;
	const wchar_t Hborder[6] = { 0x250f ,0x2501,0x2513, 0x2503 ,0x251b,0x2517 }; // heavy border;

	const wchar_t Hvborder[6] = {};
	const wchar_t Hhborder[6] = {};

	const wchar_t b_ = L'[';
	const wchar_t _e = L']';

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    VECTOR BUILDER
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	template<typename T>
	class VectorBuilder {
		std::vector<T> vec;

	public:

		VectorBuilder& add(const T& item) {
			vec.push_back(item);
			return *this;
		}

		std::vector<T> get() {
			return std::move(vec);
		}
	};

	
	
	


	
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//				MENU UTILITY    
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
		requires std::is_arithmetic_v<T>
	void get_maximum(T& max, const T& var) {
		max = var > max ? var : max;
	}

	template<typename T>
		requires std::is_arithmetic_v<T>
	void get_minimum(T& min, const T& var) {
		min = var < min ? var : min;
	}

	


}// namespace console
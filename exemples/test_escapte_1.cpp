#include <cstdio>
#include <sstream>
#include<include/console/Escape_macros.h>
#include<console project/console/ka_color.hpp>




int main() {

	std::stringstream   str;

	str << MOVETO(10, 10);
	str << COLOR(color::Colorfg16::Blue, "Hello");
	puts(str.str().c_str());
	str.str("");
	str << COLOR256(color::Color256::Gold3, "World");
	puts(str.str().c_str());



	return 0;
}

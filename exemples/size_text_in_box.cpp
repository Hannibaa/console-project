#include <iostream>
#include <include/console/CConsole_utility.h>










int main() {

	std::wstring w = LR"(hecked by the compiler and run faster.
In this book, I aim for completeness. I describe every language feature and standard-library
component that a professional programmer is likely to need. For each, I provide:
• Rationale: What kinds of problems is it designed to help solve? What principles underlie
the design? What are the fundamental limitations?
• Specification: What is its definition? The level of detail is chosen for the expert programmer;
the aspiring language lawyer can follow the many references to the ISO standard.
• Examples: How can it be used well by itself and in combination with other features? What
are the key techniques and idioms? What are the implications for maintainability and performance?
The use of C++ has changed dramatically over the years and so has the language itself. From the
point of view of a programmer, most of the changes have been improvements. The current ISO
standard C++ (ISO/IEC 14882-2011, usually called C++11) is simply a far better tool for writing
quality software than were previous versions. How is it a better tool? What kinds of programming
styles and techniques does modern C++ support? What language and standard-library features support)";
    
	// remove all \n and \r 
	console::STR::replace_all_by(w, L'\n', L' ');

	auto ww = console::STR::size_string(w, 6);
	std::cout << "size of : w " << w.size() << '\n';
	std::wcout << "String :|" << w << "|\n";
	std::wcout << "second string \n";
	std::cin.get();
	std::cout << "size of : " << ww.size() << '\n';
	std::wcout << "|" << ww << "|\n";


	return 0;
}
#include <iostream>
#include <project/RandomBox/random_box.hpp>


int main() {

	std::cout << "----------------------------------------------------------\n";
	std::cout << "- Game Start !\n";
	std::cout << "----------------------------------------------------------\n";


	game(145, 35, 8, 14);

	std::cout << "----------------- Game Over -------------------------\n";

	return 0;
}
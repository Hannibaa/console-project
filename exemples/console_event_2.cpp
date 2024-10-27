#include <iostream>

#include <include/console/console_event.hpp>






int main() {

	System::Event event{};

	Event::ThreadPullEvent(event);

	while (true) {


		if (event.text.unicode == 13) break;

		if (event.type == System::Event::KeyPressed) {
			std::cout << "key pressed \n";
			std::cout << "key is " << event.text.unicode << '\n';
		}

		if (event.type == System::Event::KeyReleased) {
			std::cout << "key released \n";
			std::cout << "key is " << event.text.unicode << '\n';
		}

	}

	Event::StopPullEvent();
	return 0;
}
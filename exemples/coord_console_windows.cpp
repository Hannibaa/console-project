#include <Windows.h>
#include <iostream>


RECT  get_console_window_rect() {
	HWND hwind = GetConsoleWindow();

	RECT rect{};
	GetWindowRect(hwind, &rect);

	return rect;
}


void move_console_window(int x, int y) {
	HWND hwind = GetConsoleWindow();

	RECT rect{};
	GetWindowRect(hwind, &rect);

	MoveWindow(hwind, x, y, rect.right - rect.left, rect.bottom - rect.top, true);
}

int main() {

	auto rect = get_console_window_rect();

	std::cout << "position [" << rect.left << "|" << rect.top << "]\n";
	for (;;) {
		Sleep(400);
		if (rect.left > 1 && rect.top > 1)
			move_console_window(--rect.left, --rect.top);
		else
			break;
	}
	return 0;
}

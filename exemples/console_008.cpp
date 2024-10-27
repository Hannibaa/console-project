#include <console project/console/ka_utility.hpp>
#include <iostream>
#include <include/time/Game_timer.h>


// function draw line 
void DrawLine(HDC& hdc, const iVec2 p1, const iVec2 p2, COLORREF color) {
	// draw line
	MoveToEx(hdc, p1.x, p1.y, NULL);
	LineTo(hdc, p2.x, p2.y);

}

int main() {
	std::cout << "draw line\n";
	//konsole->construct_console(145, 35, 8, 14);
	HWND hwnd = GetConsoleWindow();
	HDC  hdc = GetDC(hwnd);
	HPEN hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	HPEN hpenOld =(HPEN) SelectObject(hdc, hpen);

	Time::Game_Timer timer;
	while (konsole->pull_event()) {

		KA_MAKE_FPS(timer);

		PRINT_TITLE(L"Application fps[%5.2f]", ka_fps);

		POINT p;

		GetCursorPos(&p);

		auto mouse = iVec2(p.x, p.y);

	     DrawLine(hdc, { 0,0 }, mouse , RGB(255, 0, 0));

		 SelectObject(hdc, hpenOld);
		 DeleteObject(hpen);

	}


	std::cin.get();
	return 0;
}
#include <Windows.h>
#include <iostream>

// TODO : make library for windows(only win32 api) and macros
/*
						MAKE CONSOLE API LIBRARY CSL FOR WINDOWS AND MACOS!
						USING ONLY WINDOWS API FUNCTION AVAILABLE. 

*/

#define End  '\n'

void set_console_color(HANDLE hConsole, int r, int g, int b, int bgcolor = 0) {
	// Generate a color attribute from RGB values
	int color = (r > 128 ? FOREGROUND_RED :  0 ) |
				(g > 128 ? FOREGROUND_GREEN : 0) |
				(b > 128 ? FOREGROUND_BLUE : 0 ) |
				FOREGROUND_INTENSITY;

	SetConsoleTextAttribute(hConsole, bgcolor | color);
}


void print_color(const std::string& text, int r, int g, int b) {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	set_console_color(hConsole, r, g, b);
	std::cout << text << End;
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE  );
}

void print_color_at(int x, int y, const std::string& text, int r, int g, int b) {
	COORD pos;
	pos.X = x;
	pos.Y = y;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	set_console_color(hConsole, r, g, b);
	SetConsoleCursorPosition(hConsole, pos);

	std::cout << text << End;

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void print_color_at(int x, int y, const std::string& text, int r, int g, int b, int bg_color) {
	COORD pos;
	pos.X = x;
	pos.Y = y;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	set_console_color(hConsole, r, g, b, bg_color);
	SetConsoleCursorPosition(hConsole, pos);

	std::cout << text << End;

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void gotoXY(int x, int y) {
	COORD pos;
	pos.X = x;
	pos.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void enable_VTMode() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//check
	if (hConsole == INVALID_HANDLE_VALUE) {
		std::cerr << "Error : Unable to get standard output handle\n";
		exit(EXIT_FAILURE);
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hConsole, &dwMode)) {
		std::cerr << "Error : Unable to get console mode\n";
		exit(EXIT_FAILURE);
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hConsole, dwMode)) {
		std::cerr << "Error : Unable to set console mode\n";
		exit(EXIT_FAILURE);
	}
}


struct Console {
	// Get console handle 
	static HANDLE  hConsole ;

	static void set_console_color_attributes(WORD attributes) {
		SetConsoleTextAttribute(hConsole, attributes);
	}

	static bool set_console_color(COLORREF color, WORD index) {
		CONSOLE_SCREEN_BUFFER_INFOEX  csbiex;
		csbiex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

		if (!GetConsoleScreenBufferInfoEx(hConsole, &csbiex)) return false;

		csbiex.ColorTable[index] = color;

		if (!SetConsoleScreenBufferInfoEx(hConsole, &csbiex)) return false;

		return true;
	}
};

HANDLE Console::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


void print_bg_text_at(int x, int y, const std::string& text, int r, int g, int b) {

	const WORD bgIndex = 240; // Using high index to avoid conflicts with default colors
	COLORREF bgcolor = RGB(r, g, b);
	if (Console::set_console_color(bgcolor, bgIndex)) {
		Console::set_console_color_attributes(bgIndex);
		gotoXY(x, y);
		std::cout << text << End;
		// reset color to default
		Console::set_console_color_attributes(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
	else {
		std::cerr << "Failure to set custom console color.\n";
	}

}

void print_bg_text_at2(int x, int y, const std::string& text, int r, int g, int b) {

}

int main() {

	print_color("Hello Color", 0, 127, 89);
	print_color("Hello Color", 255, 0, 255);

	print_color_at(60, 10, "text at this place", 100, 200, 0);
	print_color_at(60, 20, "text bgcolor", 255, 255, 0, 0x002a  );

	//std::cin.get();
	return 0;
}
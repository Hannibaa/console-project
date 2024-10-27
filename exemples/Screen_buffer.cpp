#include <windows.h>
#include <iostream>

#define End  '\n'

void print_screen_buffer_info(CONSOLE_SCREEN_BUFFER_INFOEX* csbiex) {

	std::cout << "========================= Console Screen Buffer Info ===================================\n";
	std::cout << "Size of structure : " << csbiex->cbSize << End;
	std::cout << "Size Screen Buffer x[" << csbiex->dwSize.X << "] y[" << csbiex->dwSize.Y << "]\n";
	std::cout << "Coordinate of cursor [" << csbiex->dwCursorPosition.X << "|" 
		                                  << csbiex->dwCursorPosition.Y << "]\n";
	std::cout << "Attribute : Colors : " << csbiex->wAttributes << End;
	std::cout << "Coordinate of display window : " <<
		"Top :   " << csbiex->srWindow.Top << "  " <<
		"Left:   " << csbiex->srWindow.Left << "  " <<
		"Right:  " << csbiex->srWindow.Right << "  "
		"Bottom: " << csbiex->srWindow.Bottom << End;

	std::cout << "Maximum Wind Size " << csbiex->dwMaximumWindowSize.X << "|" <<
		csbiex->dwMaximumWindowSize.Y << End;

	std::cout << "Popup attributes : " << csbiex->wPopupAttributes << End;
	std::cout << "full screen supported : " << csbiex->bFullscreenSupported << End;
	std::cout << "color table : " << End;
	for (int i = 0; i < 16; ++i)
	{
		std::cout << "Color[" << i  << "] = " << csbiex->ColorTable[i] << "  ";
		if ((i + 1) % 4 == 0) std::cout << End;
	}
	std::cout << "========================= Console Screen Buffer Info ===================================\n";
}

auto get_console_screen_buffer_info(bool display_info = false) {

	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFOEX   csbiex;
	csbiex.cbSize = sizeof(decltype(csbiex));

	GetConsoleScreenBufferInfoEx(hstdout, &csbiex);

	if (display_info)
	print_screen_buffer_info(&csbiex);

	return csbiex;
}

int set_console_screen_buffer(int xChars, int yLines, SMALL_RECT srWindows = {0,0,0,0}) {

	HANDLE  hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hstdout == INVALID_HANDLE_VALUE) {
		std::cerr << "Error : GetStdHandle\n";
		return 0;
	}

	// Get buffer
	CONSOLE_SCREEN_BUFFER_INFOEX   csb;
	csb.cbSize = sizeof(decltype(csb));

	GetConsoleScreenBufferInfoEx(hstdout, &csb);

	// Set new parameter :
	csb.dwSize.X = xChars;
	csb.dwSize.Y = yLines;

	// Set windows console size
	if (srWindows.Bottom == 0 || srWindows.Right == 0); else {
		csb.srWindow.Right = srWindows.Right;
		csb.srWindow.Bottom = srWindows.Bottom;
	}
	
	return SetConsoleScreenBufferInfoEx(hstdout, &csb);
}



int main() {

	auto csb = get_console_screen_buffer_info();

	std::cout << " csb size : " << csb.cbSize << End;

	set_console_screen_buffer(145, 45);

	return 0;
}
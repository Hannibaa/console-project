#include <Windows.h>
#include <iostream>

#define   End    '\n'

std::pair<BYTE,BYTE> get_vk(char c) {

	short key = VkKeyScanA(c);

	return std::pair<BYTE, BYTE>(LOBYTE(key), HIBYTE(key));
}

void print_console_font_infoex(CONSOLE_FONT_INFOEX* cfi) {

	std::cout << "----------------- font information --------------------------------\n";
	std::cout << "structure size   : " << cfi->cbSize << End;
	std::cout << "Index fo font    : " << cfi->nFont << End;
	std::cout << "Font size        : " << cfi->dwFontSize.X << "|" << cfi->dwFontSize.Y << End;
	std::cout << "Font family      : " << cfi->FontFamily << End;
	std::cout << "Font Weight      : " << cfi->FontWeight << End;
	std::wcout << "Name of typeface : " << cfi->FaceName << End;
	std::cout << "----------------- font information --------------------------------\n";
}

auto get_console_font_infoex(int bMaximumWind = 0) {

	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(decltype(cfi));

	GetCurrentConsoleFontEx(hstdout, bMaximumWind, &cfi);

	return cfi;
}



int main() {
	auto key = get_vk(13);

	std::cout << "vk is " << int(key.first) << End;

	if (key.second & 1) std::cout << "Need shift\n";
	if (key.second & 2) std::cout << "Need Ctrl\n";
	if (key.second & 4) std::cout << "Need Alt\n";

	auto font = get_console_font_infoex(1);

	print_console_font_infoex(&font);

	font = get_console_font_infoex(0);

	print_console_font_infoex(&font);



	return 0;
}




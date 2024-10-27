#include <Windows.h>
#include <iostream>


#define    End  '\n'




// print all font console info to screen
void print_console_fontex(CONSOLE_FONT_INFOEX* cfi) {

	std::cout << "cbSize : size of structure : " << cfi->cbSize << End;
	std::cout << "nFont  : the index of font : " << cfi->nFont << End;
	std::cout << "Size of font               : " << cfi->dwFontSize.X << "|" << cfi->dwFontSize.Y << End;
	std::cout << "Font Family                : " << cfi->FontFamily << End;
	std::cout << "Font Weight                : " << cfi->FontWeight << End;
	std::wcout << "Font Name                  : " << cfi->FaceName << End;
}

void print_console_fontex() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(decltype(cfi));

	GetCurrentConsoleFontEx(console,FALSE, &cfi);

	print_console_fontex(&cfi);
}

void set_console_size(int width, int height) {

	HANDLE  console = GetStdHandle(STD_OUTPUT_HANDLE);

	// set screen buffer size 
	COORD bufferSize = { (short)width, (short)height };
	SetConsoleScreenBufferSize(console, bufferSize);

	// Set the windows size 
	SMALL_RECT srWind = { 0, 0, (short)(width - 1), (short)(height - 1) };
	SetConsoleWindowInfo(console, TRUE, &srWind);
}

void set_console_font_size(short wfont, short hfont) {

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);

	GetCurrentConsoleFontEx(console, FALSE, &cfi);

	// display all font info: 
	print_console_fontex(&cfi);

	// set new font
	cfi.dwFontSize.X = wfont;
	cfi.dwFontSize.Y = hfont;

	SetCurrentConsoleFontEx(console, FALSE, &cfi);

}


int set_console_font_family_weight(unsigned int FontFamily, unsigned int FontWeight = -1) {

	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX cfont;
	cfont.cbSize = sizeof(cfont);

	GetCurrentConsoleFontEx(hstdout, FALSE, &cfont);

	cfont.FontFamily = FontFamily;
	if (FontWeight != -1) cfont.FontWeight = FontWeight;

	return SetCurrentConsoleFontEx(hstdout, FALSE, &cfont);
}




// function test for set and prepare the console :
int main() {

	set_console_size(145, 45);
	set_console_font_size(0, 16);

	set_console_font_family_weight(FF_SCRIPT, FW_BOLD);

	print_console_fontex();

	return 0;
}
















/*				Demonstrate how to use close handle					*/	
int main1() {

	HANDLE   StdIn;
	HANDLE   StdOut = GetStdHandle(STD_OUTPUT_HANDLE);


	std::cin.get();
	std::cout << "close handle" << End;
	std::cout << "not close " << End;
	std::cin.get();
	return 0;
}
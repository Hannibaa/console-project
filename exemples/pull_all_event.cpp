#include <Windows.h>
#include <iostream>
#include <thread>
#include <conio.h>
#include <include/console/console.hpp>

#define   End   '\n'
#define   _CTRL_C   3

HANDLE hstdin;
DWORD  dwOldMode;
static bool   bStop = false;

// this will illiminate mouse input record /////////////////////
//static void LunchThreadStop(int stop_constrol) {
//	std::thread([=] {
//		while (!bStop) {
//			if (_getch() == stop_constrol) {
//				bStop = true;
//			}
//		}
//		}).detach();
//}

void Error(const char* msg, int saveMode = 0) {
	fprintf(stderr, "%s : %d\n", msg, GetLastError() );

	if (saveMode) SetConsoleMode(hstdin, dwOldMode);

	ExitProcess(0);
}


void key_event_proc(KEY_EVENT_RECORD* ker);
void mouse_event_proc(MOUSE_EVENT_RECORD* mer);
void window_event_proc(WINDOW_BUFFER_SIZE_RECORD* wer);
void focus_event_proc(FOCUS_EVENT_RECORD* fer);
void menu_event_proc(MENU_EVENT_RECORD* mner);




int main() {

	console::set_console_size(145, 45);
	console::set_console_font_size(0, 16);

	DWORD  dwMode,  nEvents, i{};
	INPUT_RECORD  irb[128];

	//LunchThreadStop(_CTRL_C);

	hstdin = GetStdHandle(STD_INPUT_HANDLE);

	if (hstdin == INVALID_HANDLE_VALUE) {
		Error("GetStdHandle");
	}

	// get console mode and save it 
	if (!GetConsoleMode(hstdin, &dwOldMode)) {
		Error("GetConsoleMode");
	}

	// set new mode :
	dwMode =  ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;

	if (!SetConsoleMode(hstdin, dwMode)) {
		Error("SetConsoleMode", 1);
	}


	while (!bStop) {
		
		// Wait for event 
		if (!ReadConsoleInput(
			hstdin,
			irb,
			128,
			&nEvents
		))
			Error("ReadConsoleInput", 1);

		// print info
		std::wcout << "nEvents read " << nEvents << End;

		// Dispathc the events to the appropriate handler
		for (i = 0; i < nEvents; i++)
		{
			switch (irb[i].EventType) 
			{
				case KEY_EVENT:
					key_event_proc(&irb[i].Event.KeyEvent);
					break;

				case MOUSE_EVENT:
					std::cout << "mouse event\n";
					mouse_event_proc(&irb[i].Event.MouseEvent);
					break;

				case WINDOW_BUFFER_SIZE_EVENT:
					window_event_proc(&irb[i].Event.WindowBufferSizeEvent);
					break;

				case FOCUS_EVENT:
					focus_event_proc(&irb[i].Event.FocusEvent);
					break;

				case MENU_EVENT:
					menu_event_proc(&irb[i].Event.MenuEvent);
					break;

			}
		}
	}



	SetConsoleMode(hstdin, dwOldMode);

	return 0;
}



void key_event_proc(KEY_EVENT_RECORD* ker) {
	
	std::cout << "is Key down       : " << ker->bKeyDown << End;
	std::cout << "repeat count      :" << ker->wRepeatCount << End;
	std::cout << "Virtual key  code :" << ker->wVirtualKeyCode << End;
	std::cout << "Virtual scan code :" << ker->wVirtualScanCode << End;
	std::cout << "Ascii char        :" << ker->uChar.AsciiChar << End;
	std::wcout << "Unicode char      :" << ker->uChar.UnicodeChar << End;
	std::cout << "Constrol key stat :" << ker->dwControlKeyState << End;
	std::cout << "--------------------------------------------------------\n";
}


void mouse_event_proc(MOUSE_EVENT_RECORD* mer) {

	std::cout << "mouse position x[" << mer->dwMousePosition.X
		<< "] y[" << mer->dwMousePosition.Y << "]\n";
	std::cout << "Button stat      : " << mer->dwButtonState << End;
	std::cout << "control key stat :" << mer->dwControlKeyState << End;
	std::cout << "event flags      : " << mer->dwEventFlags << End;

}


void window_event_proc(WINDOW_BUFFER_SIZE_RECORD* wer) {
	std::cout << "window size : [" << wer->dwSize.X << "|" << wer->dwSize.Y << "]\n";
}

void focus_event_proc(FOCUS_EVENT_RECORD* fer) {
	std::cout << "is Window focus : " << fer->bSetFocus << End;
}

void menu_event_proc(MENU_EVENT_RECORD* mner) {
	std::cout << "menu command id : " << mner->dwCommandId << End;
}





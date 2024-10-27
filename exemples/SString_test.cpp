#include <console project/component/ka_SString.hpp>
#include <console project/console/ka_color.hpp>
#include <console project/component/TextUI/ka_StatuBar.hpp>

struct Button {
	Button* next{ nullptr };
	// repetition
	short OnMouseRep{};
	short OnClickRep{};
	short OnReleasedRep{};
	// logic
	bool OnMouse{false};
	bool OnClick{false};
	bool OnDoubleClick{false};
	bool OnReleased{ true };
};

void button(WSString& str, const fVec2& mouse, Button& _button,
	int OnClick_colorfg,
	int OnClick_colorbg,
	int OnMouse_colorbg,
	int OnMouse_colorfg)
{
	// get Old color
	auto Old_color = str.get_originalColor();

	if (str.get_bounds().contain(mouse)) {
		_button.OnMouse = true;
		str.set_colorBG(OnMouse_colorbg);
		str.set_colorFG(OnMouse_colorfg);

		if (KeyPressed(VK_LBUTTON)) {
			_button.OnClick = true;
			_button.OnReleased = false;
			str.set_colorFG(OnClick_colorfg);
			str.set_colorBG(OnClick_colorbg);
		}
		else
		{
			_button.OnClick = false;
			//_button.OnReleased = true;
		}
		
		if (KeyReleased(VK_LBUTTON))
		{
			++_button.OnReleasedRep;
			_button.OnReleased = true;
			str.set_colorFG(Old_color.first);
			str.set_colorBG(Old_color.second);

		}
		
	}
	else
	{
		_button.OnMouse = false;
		str.set_colorFG(Old_color.first);
		str.set_colorBG(Old_color.second);
	}

}



int main() {

	const int cwidth = 145;
	const int cheight = 35;
	const int cwfont = 8;
	const int chfont = 14;
	const int index = 2;
	const float Velocity = 0.6f;
	// construct window console
	konsole->construct_console(cwidth, cheight, cwfont, chfont);
	// set drawable window
	konsole->set_drawWindows(0.f, 0.f, cwidth, cheight - index);
	// set background color
	konsole->set_background_color(color::Blue << 4);


	////////////////////////////////////////////////////////// Data

	console::SString<wchar_t>    str(L"hello world", {color::Black, color::Red});

	str.set_position(20, 20);







//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    Main Loop
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	Time::Game_Timer timer;
	int count{};
	int minutes{};
	fVec2 mouse{};
	Button _button;

	while (konsole->is_open()) {
		konsole->clear();

		KA_MAKE_FPS(timer);
		KA_MAKE_DALTA_TIME(1);
		minutes = static_cast<int>(timer.time_since_begin());

		while (konsole->pull_event()) {
			//++count;
			mouse = fVec2(konsole->get_mouseXY());
		}


		PRINT_TITLE(L"SString Test -- Kadda Aoues fps[%4.2f] minutes[%d] Mouse[%d|%d] -- %d -- %d",
			ka_fps, minutes, int(mouse.x), int(mouse.y), _button.OnReleasedRep, count);


		button(str, mouse,_button,
			color::Green,
			color::GrayDark,
			color::Yellow,
			color::Cyan
			);


		TextUI::StatuBar(TextUI::StatuBarBuilder{}
			.set_colorBG(color::Black << 4)
			.set_colorFG(color::White)
			.set_colorChar(color::Red)
			.set_position(0, cheight - 1)
			.set_step(2, 0)
			.add_item(L"Esc Exit")
			.add_item(L"M Mouse")
			.get());

		str.draw();
		konsole->display();
	}

	return 0;
}
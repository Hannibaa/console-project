#pragma once


// make cursor coodonate in statubar :

/*
* 
	making structure :
	------------------
	struct Option
	{
		bool			action;
		std::wstring	comment;
	};
	
	using CustomOption = std::map<std::string, Option>;

	
	to display a cursor :
	---------------------


	std::wstring cursor(9, L' ');
	if (option["ShowCursor"].action)
	{
		swprintf_s(cursor.data(), 8, L"%3d|%2d", int(mouse.x), int(mouse.y));
		_action = L"Show Cursor";
	}


	to display date and time :
	--------------------------

	std::wstring sTime(10, L' ');
	std::wstring sDate(10, L' ');


	if (option["ShowDate"].action)
	{
		sDate = console::make_current_date();
	}

	if (option["ShowTime"].action) {
		sTime = console::make_current_time();
	}

	all all item in statu bar function :
	------------------------------------

			.add_item(L"Time", 0 ,option["ShowTime"].action )
			.add_item(L"Date", 0 ,option["ShowDate"].action )
			.add_item(L"Cursor", 0 , option["ShowCursor"].action)
			.add_item(cursor,W-7, false,3)
			.add_item(sDate,W - sDate.size() - 8, false,0,3 )
			.add_item(sTime,W - sTime.size() - 24, false,0,2)


*/



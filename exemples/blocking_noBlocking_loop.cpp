#include <windows.h>
#include <iostream>	


#define End '\n'



int main() {
	DWORD nEvents{};
	INPUT_RECORD irb[32];
	HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
	int i{};


	while (
		ReadConsoleInput(hstdin, irb, 32, &nEvents)
		)
	{

		std::cout << "We have record input " << nEvents << " records is : " << ++i << End;

		if (nEvents > 31 )
		 FlushConsoleInputBuffer(hstdin);
	}

	//while (
	//	PeekConsoleInput(hstdin, irb, 32, &nEvents)
	//	)
	//{
	//	std::cout << "We have record input " << nEvents << " records is : " << ++i << End;



	//	if (nEvents > 31 )
	//	 FlushConsoleInputBuffer(hstdin);
	//}



	return 0;
}

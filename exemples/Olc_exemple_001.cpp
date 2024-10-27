#include <include/console/olcConsoleGameEngine.h>


class exemple :public olcConsoleGameEngine {


public:



	virtual bool OnUserCreate() override {

		

		return true;
	}

	virtual bool OnUserUpdate(float fps) {

        int x = GetMouseX();
		int y = GetMouseY();


		Fill(x, y, 20 + x, 20 + y);
		

		

		return true;
	}
};



int main() {

	exemple   game;

	game.ConstructConsole(145, 45, 4, 4);

	game.Start();

	return 0;
}
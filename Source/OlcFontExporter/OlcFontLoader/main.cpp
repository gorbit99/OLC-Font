#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"
#include "olcPGEX_Fonts.h"

class Engine : public olc::PixelGameEngine {
public:

	olc::Font testFont = olc::Font("output.of");

	Engine() {}

	bool OnUserCreate() {
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) {
		Clear(olc::BLACK);
		testFont.DrawString(20, 20, "Font stolen off of OpenGameArt!");
		return true;
	}
};

int main() {
	Engine engine;
	engine.Construct(320, 240, 2, 2);
	engine.Start();
	return 0;
}
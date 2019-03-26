#include "Engine.h"

class Test : public Engine
{
public:
	void OnCreate() override
	{
		ShowFps(true);
	}

	void OnUpdate(float dt) override
	{
		FillWindowWinAPI(Colors.WHITE.ToCOLORREF());

		//1 stage +
		DrawLine(200, 200, 250, 225, Colors.BLACK);
		DrawLine(200, 200, 250, 175, Colors.BLACK);

		//2 stage +
		DrawLine(200, 200, 250, 275, Colors.BLACK);
		DrawLine(200, 200, 250, 125, Colors.BLACK);

		//3 stage -
		DrawLine(200, 200, 150, 225, Colors.BLACK);
		DrawLine(200, 200, 150, 175, Colors.BLACK);
		DrawLine(200, 200, 150, 275, Colors.BLACK);
		DrawLine(200, 200, 150, 125, Colors.BLACK);

		DrawLine(200, 200, 250, 200, Colors.BLACK);
		DrawLine(200, 200, 150, 200, Colors.BLACK);
		DrawLine(200, 200, 200, 250, Colors.BLACK);
		DrawLine(200, 200, 200, 150, Colors.BLACK);
		
		DrawLine(200, 200, 250, 250, Colors.BLACK);
		DrawLine(200, 200, 150, 150, Colors.BLACK);
		DrawLine(200, 200, 250, 150, Colors.BLACK);
		DrawLine(200, 200, 150, 250, Colors.BLACK);
	}
};

int main()
{
	Test eng;
	eng.Start(1024, 768);

	return 0;
}
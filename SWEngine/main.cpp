#include "Engine.h"

class Test : public Engine
{
	void OnUpdate(float dt) override
	{
		FillTriangle(450,500,500,300,550,500, HEXColorFormat("#ff3d3d"));
		DrawCircle(200,200,50, HEXColorFormat("#ff3d3d"));
	}
};

int main()
{
	Test eng;
	eng.Start(1024,768);

	return 0;
}
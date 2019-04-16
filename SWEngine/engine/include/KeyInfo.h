#pragma once

namespace SWEngine
{
	class KeyInfo
	{
	public:
		KeyInfo();
		~KeyInfo();

		bool KeyDown;
		bool KeyUp;
		bool KeyPressed;
	};
}
#include "../../include/gui/Button.h"
#include "../../include/Engine.h"

namespace SWE
{
	namespace GUI
	{
		Button::Button(const int& x, const int& y, const int& width, const int& height) : UIElement(x, y, width, height) {}
		Button::~Button() {}
		void Button::UpdateElement(void* engine)
		{
			if (IsMouseHover())
			{
				((Engine*)engine)->FillRectangleWinAPI(x, y, x + width, y + height, mouseHoverColor);
			}
			else ((Engine*)engine)->FillRectangleWinAPI(x, y, x + width, y + height, backgroundColor);
			((Engine*)engine)->DrawStringWinAPI(text.c_str(), x, y, x + width, y + height, foregroundColor);
		}
	}
}
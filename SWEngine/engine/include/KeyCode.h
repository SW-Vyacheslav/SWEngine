#ifndef SWE_KEYCODE
#define SWE_KEYCODE

#include "Export.h"

namespace SWE
{
	class SWE_API KeyCode
	{
	public:
		KeyCode();
		~KeyCode();

		static const unsigned char A;
		static const unsigned char B;
		static const unsigned char C;
		static const unsigned char D;
		static const unsigned char E;
		static const unsigned char F;
		static const unsigned char G;
		static const unsigned char H;
		static const unsigned char I;
		static const unsigned char J;
		static const unsigned char K;
		static const unsigned char L;
		static const unsigned char M;
		static const unsigned char N;
		static const unsigned char O;
		static const unsigned char P;
		static const unsigned char Q;
		static const unsigned char R;
		static const unsigned char S;
		static const unsigned char T;
		static const unsigned char U;
		static const unsigned char V;
		static const unsigned char W;
		static const unsigned char X;
		static const unsigned char Y;
		static const unsigned char Z;
		static const unsigned char MOUSELEFT;
		static const unsigned char MOUSERIGHT;
		static const unsigned char MOUSEMIDDLE;
		static const unsigned char BACKSPACE;
		static const unsigned char TAB;
		static const unsigned char ENTER;
		static const unsigned char SHIFT;
		static const unsigned char CTRL;
		static const unsigned char ALT;
		static const unsigned char ESC;
		static const unsigned char SPACE;
		static const unsigned char LEFTARROW;
		static const unsigned char UPARROW;
		static const unsigned char RIGHTARROW;
		static const unsigned char DOWNARROW;
	};
}

#endif
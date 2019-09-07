#ifndef SWE_INPUT
#define SWE_INPUT

#include "../Export.h"
#include "KeyInfo.h"
#include "KeyCode.h"
#include "../ui/Window.h"
#include "../system/ScreenPoint.h"

namespace swe
{
	namespace system
	{
		class SWE_API Input
		{
		public:
			static Bool IsKeyDown(const KeyCode& keyCode);
			static Bool IsKeyUp(const KeyCode& keyCode);
			static Bool IsKeyPressed(const KeyCode& keyCode);
			static ScreenPoint GetMousePossition();

			friend ui::Window;

		private:
			Input();
			~Input();

			static system::KeyInfo m_keys[256];
			static Int16 m_keysNewState[256];
			static Int16 m_keysOldState[256];
		};
	}
}

#endif
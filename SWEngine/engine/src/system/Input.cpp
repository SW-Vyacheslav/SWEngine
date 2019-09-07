#include "../../include/system/Input.h"
#include <Windows.h>

namespace swe
{
	namespace system
	{
		Input::Input(){}
		Input::~Input(){}

		Bool Input::IsKeyDown(const KeyCode& keyCode)
		{
			return m_keys[keyCode].KeyDown;
		}

		Bool Input::IsKeyUp(const KeyCode& keyCode)
		{
			return m_keys[keyCode].KeyUp;
		}

		Bool Input::IsKeyPressed(const KeyCode& keyCode)
		{
			return m_keys[keyCode].KeyPressed;
		}

		ScreenPoint Input::GetMousePossition()
		{
			POINT mousePos;
			GetCursorPos(&mousePos);
			ScreenToClient(ui::Window::GetInstance()->GetHandle(), &mousePos);
			return ScreenPoint();
		}

		system::KeyInfo Input::m_keys[256] = {};
		Int16 Input::m_keysNewState[256] = {};
		Int16 Input::m_keysOldState[256] = {};
	}
}
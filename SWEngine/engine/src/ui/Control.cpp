#include "../../include/ui/Control.h"

namespace swe 
{
	namespace ui
	{
		Control::Control() {}
		Control::~Control() {}

		Bool Control::Create(Int32 id, Int32 width, Int32 height, system::ScreenPoint position)
		{
			return true;
		}

		void Control::Enable(Bool bEnable)
		{
			EnableWindow(m_hWnd, bEnable);
		}

		HWND Control::GetHandle() const
		{
			return m_hWnd;
		}
	}
}
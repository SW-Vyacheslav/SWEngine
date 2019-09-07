#ifndef SWE_CONTROL
#define SWE_CONTROL

#include "../Export.h"
#include "../system/ScreenPoint.h"
#include <Windows.h>

namespace swe
{
	namespace ui
	{
		class SWE_API Control
		{
		public:
			Control();
			~Control();

			Bool Create(Int32 id, Int32 width, Int32 height, system::ScreenPoint position);

			void Enable(Bool bEnable);

			HWND GetHandle() const;

		protected:
			HWND m_hWnd;
			Int32 m_id;
		};
	}
}

#endif
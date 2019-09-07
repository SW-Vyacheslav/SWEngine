#ifndef SWE_WINDOW
#define SWE_WINDOW

#include "../Export.h"
#include "Control.h"
#include <Windows.h>
#include <vector>

namespace swe
{
	namespace ui
	{
		class SWE_API Window
		{
		public:
			bool Create(const std::wstring& title, UInt32 width, UInt32 height);
			void Show();
			void Destroy();

			void AddControl(const Control& control);
			void SetTitle(const std::wstring& title);
			
			std::wstring GetTitle() const;
			HWND GetHandle() const;
			static Window* GetInstance();

		private:
			Window();
			~Window();
			Window(const Window& window);
			Window& operator=(Window& window) = delete;

			void InputProc();

		private:
			static Window* m_pWindowInstance;
			HWND m_hWnd;
			ULONG_PTR m_gdiplusToken;
			std::wstring m_sTitle;
			std::vector<Control> m_vControls;
		};
	}
}

#endif
#include "../../include/ui/Window.h"
#include <gdiplus.h>
#include <objidl.h>
#include "../../include/system/Input.h"

#pragma comment (lib,"Gdiplus.lib")

namespace swe
{
	namespace ui
	{
		Window::Window()
		{
			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
			GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
		}

		Window::~Window()
		{
			Gdiplus::GdiplusShutdown(m_gdiplusToken);
		}

		static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_CLOSE:
			{
				PostQuitMessage(0);
				break;
			}

			default:
				return DefWindowProc(hWnd, msg, wParam, lParam);
			}

			return FALSE;
		}

		bool Window::Create(const std::wstring& title, UInt32 width, UInt32 height)
		{
			WNDCLASSEX wce;
			wce.cbClsExtra = NULL;
			wce.cbSize = sizeof(WNDCLASSEX);
			wce.cbWndExtra = NULL;
			wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wce.hCursor = LoadCursor(NULL, IDC_ARROW);
			wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wce.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
			wce.hInstance = GetModuleHandle(0);
			wce.lpfnWndProc = MainWndProc;
			wce.lpszMenuName = NULL;
			wce.lpszClassName = L"SWEngineWindowClass";
			wce.style = CS_VREDRAW | CS_HREDRAW;

			if (!RegisterClassEx(&wce)) return false;

			m_hWnd = CreateWindow
			(
				L"SWEngineWindowClass",
				title.c_str(),
				WS_CAPTION | WS_OVERLAPPED | WS_SYSMENU,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				width,
				height,
				NULL,
				NULL,
				wce.hInstance,
				NULL
			);

			if (m_hWnd == NULL) return false;
			
			return true;
		}

		void Window::Show()
		{
			ShowWindow(m_hWnd, SW_SHOW);
			ShowWindow(GetConsoleWindow(), SW_HIDE);
			UpdateWindow(m_hWnd);

			MSG msg;

			while (GetMessage(&msg, NULL, NULL, NULL))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		void Window::Destroy()
		{
			DestroyWindow(m_hWnd);
			Gdiplus::GdiplusShutdown(m_gdiplusToken);
		}

		void Window::SetTitle(const std::wstring& title)
		{
			SetWindowText(m_hWnd, title.c_str());
			m_sTitle = title;
		}

		std::wstring Window::GetTitle() const
		{
			return m_sTitle;
		}

		void Window::InputProc()
		{
			while (true)
			{
				for (int i = 0; i < 256; i++)
				{
					system::Input::m_keysNewState[i] = GetKeyState(i);

					system::Input::m_keys[i].KeyDown = false;
					system::Input::m_keys[i].KeyUp = false;

					if (system::Input::m_keysNewState[i] != system::Input::m_keysOldState[i])
					{
						if (system::Input::m_keysNewState[i] & 0x8000)
						{
							system::Input::m_keys[i].KeyDown = !system::Input::m_keys[i].KeyPressed;
							system::Input::m_keys[i].KeyPressed = true;
						}
						else
						{
							system::Input::m_keys[i].KeyUp = true;
							system::Input::m_keys[i].KeyPressed = false;
						}
					}

					system::Input::m_keysOldState[i] = system::Input::m_keysNewState[i];
				}
				Sleep(50);
			}
		}

		void Window::AddControl(const Control& control)
		{
			SetWindowLong(control.GetHandle(), GWL_HWNDPARENT, (LONG)m_hWnd);
			m_vControls.push_back(control);
		}

		Window* Window::GetInstance()
		{
			if (!m_pWindowInstance) m_pWindowInstance = new Window();
			return m_pWindowInstance;
		}

		HWND Window::GetHandle() const
		{
			return m_hWnd;
		}

		Window* Window::m_pWindowInstance = NULL;
	}
}
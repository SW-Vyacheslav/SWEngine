#include "Engine.h"

namespace SWEngine
{
	Engine::Engine() : f_bShowFps(false), f_bAtomActive(false) {}
	VOID Engine::Start(UINT windowWidth, UINT windowHeight)
	{
		f_hInstance = GetModuleHandle(0);

		if (!f_hInstance) throw "Can't get an instance.";

		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		WNDCLASSEX wce;
		wce.cbClsExtra = NULL;
		wce.cbSize = sizeof(WNDCLASSEX);
		wce.cbWndExtra = NULL;
		wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wce.hCursor = LoadCursor(NULL, IDC_ARROW);
		wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wce.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		wce.hInstance = f_hInstance;
		wce.lpfnWndProc = StaticMainWndProc;
		wce.lpszMenuName = NULL;
		wce.lpszClassName = L"SWEngineWindowClass";
		wce.style = CS_VREDRAW | CS_HREDRAW;

		if (!RegisterClassEx(&wce)) throw "Window class is not registrated.";

		f_hMainWnd = CreateWindow
		(
			L"SWEngineWindowClass",
			L"Test",
			WS_CAPTION | WS_OVERLAPPED | WS_SYSMENU | WS_MAXIMIZEBOX,
			100,
			100,
			windowWidth,
			windowHeight,
			NULL,
			NULL,
			f_hInstance,
			this
		);

		if (!f_hMainWnd) throw "Window is not created.";

		RECT clientRect;
		GetClientRect(f_hMainWnd, &clientRect);

		f_uiClientWidth = clientRect.right;
		f_uiClientHeight = clientRect.bottom;

		f_timePoint1 = std::chrono::system_clock::now();
		f_timePoint2 = std::chrono::system_clock::now();

		f_hMainWndDC = GetDC(f_hMainWnd);
		f_hBufferDC = CreateCompatibleDC(f_hMainWndDC);
		f_hBufferBitmap = CreateCompatibleBitmap(f_hMainWndDC, GetClientWidth(), GetClientHeight());

		HBITMAP hOldBufferBitmap = (HBITMAP)SelectObject(f_hBufferDC, f_hBufferBitmap);

		SetStretchBltMode(f_hMainWndDC, COLORONCOLOR);
		SetStretchBltMode(f_hBufferDC, COLORONCOLOR);

		ShowWindow(f_hMainWnd, SW_SHOW);
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		UpdateWindow(f_hMainWnd);

		f_bAtomActive = true;
		std::thread engine_thread(&Engine::EngineLoop, this);

		MSG msg;

		while (GetMessage(&msg, NULL, NULL, NULL))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		engine_thread.join();

		Gdiplus::GdiplusShutdown(gdiplusToken);
	}
	UINT Engine::GetClientWidth() const
	{
		return f_uiClientWidth;
	}
	UINT Engine::GetClientHeight() const
	{
		return f_uiClientHeight;
	}
	INT Engine::GetMouseX() const
	{
		return f_iMousePosX;
	}
	INT Engine::GetMouseY() const
	{
		return f_iMousePosY;
	}
	VOID Engine::ShowFps(bool isShowed)
	{
		f_bShowFps = isShowed;
	}
	bool Engine::IsFpsShowed() const
	{
		return f_bShowFps;
	}
	const KeyInfo* Engine::GetKeyInfo(const INT& keycode) const
	{
		if (keycode < 0 || keycode > 255) return NULL;

		return &f_keys[keycode];
	}
	//UI methods
	VOID Engine::AddUIElement(GUI::UIElement& el)
	{
		f_vuiElements.push_back(&el);
	}
	//SWEngine drawing methods
	VOID Engine::DrawLine(const int& x1, const int& y1, const int& x2, const int& y2, const Color::IColorFormat& color)
	{
		int dx = std::abs(x2 - x1);
		int dy = std::abs(y2 - y1);

		int x = x1;
		int y = y1;
		int xend = x2;
		int yend = y2;

		if (dx == dy)
		{
			int incy = y < yend ? 1 : -1;
			int incx = x < xend ? 1 : -1;

			DrawPixelWinAPI(x, y, color.ToCOLORREF());
			while (x != xend || y != yend)
			{
				x += incx;
				y += incy;
				DrawPixelWinAPI(x, y, color.ToCOLORREF());
			}
			return;
		}
		else if (dx == 0)
		{
			int incy = y < yend ? 1 : -1;

			DrawPixelWinAPI(x, y, color.ToCOLORREF());
			while (y != yend)
			{
				y += incy;
				DrawPixelWinAPI(x, y, color.ToCOLORREF());
			}
			return;
		}
		else if (dy == 0)
		{
			int incx = x < xend ? 1 : -1;

			DrawPixelWinAPI(x, y, color.ToCOLORREF());
			while (x != xend)
			{
				x += incx;
				DrawPixelWinAPI(x, y, color.ToCOLORREF());
			}
			return;
		}

		bool inv = false;

		if (dx < dy)
		{
			inv = true;
			std::swap(dx, dy);
			std::swap(x, y);
			std::swap(xend, yend);
		}

		if (x > xend)
		{
			std::swap(x, xend);
			std::swap(y, yend);
		}

		int incy = y < yend ? 1 : -1;

		int d = 2 * dy - dx;

		DrawPixelWinAPI(inv ? yend : xend, inv ? xend : yend, color.ToCOLORREF());
		while (x != xend || y != yend)
		{
			DrawPixelWinAPI(inv ? y : x, inv ? x : y, color.ToCOLORREF());

			if (d >= 0)
			{
				y += incy;
				d -= 2 * dx;
			}

			d += 2 * dy;
			x++;
		}
	}
	VOID Engine::DrawTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const Color::IColorFormat& color)
	{
		DrawLine(x1, y1, x2, y2, color);
		DrawLine(x2, y2, x3, y3, color);
		DrawLine(x3, y3, x1, y1, color);
	}
	VOID Engine::FillTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const Color::IColorFormat& color)
	{
		
	}
	VOID Engine::FillRectangle(const int& x1, const int& y1, const int& x2, const int& y2, const Color::IColorFormat& color)
	{
		int incy = y1 < y2 ? 1 : -1;
		int y = y1;

		DrawLine(x1, y2, x2, y2, color);
		while (y != y2)
		{
			DrawLine(x1, y, x2, y, color);
			y += incy;
		}
	}
	VOID Engine::DrawCircle(const int& cx, const int& cy, const int& radius, const Color::IColorFormat& color)
	{
		int x = 0;
		int y = radius;
		int delta = 1 - 2 * radius;
		int error = 0;

		while (y >= 0)
		{
			DrawPixelWinAPI(cx + x, cy + y, color.ToCOLORREF());
			DrawPixelWinAPI(cx + x, cy - y, color.ToCOLORREF());
			DrawPixelWinAPI(cx - x, cy + y, color.ToCOLORREF());
			DrawPixelWinAPI(cx - x, cy - y, color.ToCOLORREF());
			error = 2 * (delta + y) - 1;
			if (delta < 0 && error <= 0) {
				++x;
				delta += 2 * x + 1;
				continue;
			}
			error = 2 * (delta - x) - 1;
			if (delta > 0 && error > 0) {
				--y;
				delta += 1 - 2 * y;
				continue;
			}
			++x;
			delta += 2 * (x - y);
			--y;
		}
	}
	//WinAPI drawing methods
	VOID Engine::FillCircleWinAPI(const int& cx, const int& cy, const int& radius, const COLORREF& color)
	{
		HPEN hNewPen = CreatePen(PenStyle::SOLID, 1, color);
		HPEN hOldPen = (HPEN)SelectObject(f_hBufferDC, hNewPen);
		HBRUSH hNewBrush = CreateSolidBrush(color);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(f_hBufferDC, hNewBrush);

		Ellipse(f_hBufferDC, cx - radius, cy - radius, cx + radius, cy + radius);

		SelectObject(f_hBufferDC, hOldPen);
		SelectObject(f_hBufferDC, hOldBrush);
		DeleteObject(hNewPen);
		DeleteObject(hOldBrush);
	}
	VOID Engine::DrawPixelWinAPI(const int& x, const int& y, const COLORREF& color)
	{
		SetPixel(f_hBufferDC, x, y, color);
	}
	VOID Engine::DrawLineWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const COLORREF& color, const UINT& width, const INT& pen_style)
	{
		HPEN hNewPen = CreatePen(pen_style, width, color);
		HPEN hOldPen = (HPEN)SelectObject(f_hBufferDC, hNewPen);

		MoveToEx(f_hBufferDC, x1, y1, NULL);
		LineTo(f_hBufferDC, x2, y2);

		SelectObject(f_hBufferDC, hOldPen);
		DeleteObject(hNewPen);
	}
	VOID Engine::DrawTriangleWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const COLORREF& color, const UINT& border_width, const INT& pen_style)
	{
		DrawLineWinAPI(x1, y1, x2, y2, color, border_width, pen_style);
		DrawLineWinAPI(x2, y2, x3, y3, color, border_width, pen_style);
		DrawLineWinAPI(x3, y3, x1, y1, color, border_width, pen_style);
	}
	VOID Engine::DrawStringWinAPI(LPCWSTR str, const int& x1, const int& y1, const int& x2, const int& y2, const COLORREF& textcolor)
	{
		RECT textRect = { x1,y1,x2,y2 };
		int prevmode = SetBkMode(f_hBufferDC, TRANSPARENT);
		COLORREF prevcolor = SetTextColor(f_hBufferDC, textcolor);
		HFONT newfont = CreateFont(0, 0, 0, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, ((FF_DONTCARE << 4) & DEFAULT_PITCH), L"Roboto");
		HFONT oldfont = (HFONT)SelectObject(f_hBufferDC, newfont);
		DrawText(f_hBufferDC, str, lstrlenW(str), &textRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		SetBkMode(f_hBufferDC, prevmode);
		SetTextColor(f_hBufferDC, prevcolor);
		SelectObject(f_hBufferDC, oldfont);
		DeleteObject(newfont);
	}
	VOID Engine::DrawImageWinAPI(const HBITMAP& bitmap, const int& x1, const int& y1, const UINT& width, const UINT& height)
	{
		HDC hDC = CreateCompatibleDC(f_hMainWndDC);
		SelectObject(hDC, bitmap);
		BITMAP bm;
		GetObject(bitmap, sizeof(bm), &bm);
		if (width > 0 && height > 0) StretchBlt(f_hBufferDC, x1, y1, width, height, hDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		else StretchBlt(f_hBufferDC, x1, y1, bm.bmWidth, bm.bmHeight, hDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		DeleteDC(hDC);
	}
	VOID Engine::FillTriangleWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const COLORREF& color)
	{
		HPEN hNewPen = CreatePen(PenStyle::SOLID, 1, color);
		HPEN hOldPen = (HPEN)SelectObject(f_hBufferDC, hNewPen);
		HBRUSH hNewBrush = CreateSolidBrush(color);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(f_hBufferDC, hNewBrush);

		POINT p[3];
		p[0].x = x1;
		p[0].y = y1;
		p[1].x = x2;
		p[1].y = y2;
		p[2].x = x3;
		p[2].y = y3;
		Polygon(f_hBufferDC, p, 3);

		SelectObject(f_hBufferDC, hOldPen);
		SelectObject(f_hBufferDC, hOldBrush);
		DeleteObject(hNewPen);
		DeleteObject(hOldBrush);
	}
	VOID Engine::FillRectangleWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const COLORREF& color)
	{
		HPEN hNewPen = CreatePen(PenStyle::SOLID, 1, color);
		HPEN hOldPen = (HPEN)SelectObject(f_hBufferDC, hNewPen);
		HBRUSH hNewBrush = CreateSolidBrush(color);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(f_hBufferDC, hNewBrush);

		Rectangle(f_hBufferDC, x1, y1, x2, y2);

		SelectObject(f_hBufferDC, hOldPen);
		SelectObject(f_hBufferDC, hOldBrush);
		DeleteObject(hNewPen);
		DeleteObject(hOldBrush);
	}
	VOID Engine::FillWindowWinAPI(const COLORREF& color)
	{
		FillRectangleWinAPI(0, 0, GetClientWidth(), GetClientHeight(), color);
	}
	VOID Engine::FillWindowWinAPI(const HBITMAP& bitmap)
	{
		DrawImageWinAPI(bitmap, 0, 0, GetClientWidth(), GetClientHeight());
	}
	HBITMAP Engine::LoadHBitmap(const LPCWSTR filename)
	{
		HBITMAP hbm;
		Gdiplus::Bitmap bm(filename);
		bm.GetHBITMAP(Gdiplus::Color(0, 0, 0), &hbm);

		return hbm;
	}
	VOID Engine::OnCreate() {};
	VOID Engine::OnUpdate(float fdeltaTime) {};
	VOID Engine::OnDestroy() {}
	LRESULT CALLBACK Engine::StaticMainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Engine* temp;

		if (msg == WM_CREATE)
		{
			temp = (Engine*)((LPCREATESTRUCT)lParam)->lpCreateParams;
			SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)temp);
		}
		else
		{
			temp = (Engine*)GetWindowLongPtr(hWnd, GWL_USERDATA);
			if (!temp) return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return temp->MainWndProc(hWnd, msg, wParam, lParam);
	}
	LRESULT Engine::MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_SIZE:
		{
			RECT clientRect;
			GetClientRect(f_hMainWnd, &clientRect);

			f_uiClientWidth = clientRect.right;
			f_uiClientHeight = clientRect.bottom;

			f_mutexBuffer.lock();
			DeleteObject(f_hBufferBitmap);
			f_hBufferBitmap = CreateCompatibleBitmap(f_hMainWndDC, GetClientWidth(), GetClientHeight());
			SelectObject(f_hBufferDC, f_hBufferBitmap);
			f_mutexBuffer.unlock();

			break;
		}

		case WM_SYSCOMMAND:
		{
			if (wParam == SC_CLOSE)
			{
				f_bAtomActive = false;

				std::unique_lock<std::mutex> ul(f_mutexEngineLoop);
				f_cvEngineLoopFinished.wait(ul);

				DeleteObject(f_hBufferBitmap);
				DeleteObject(f_hBufferDC);
				ReleaseDC(f_hMainWnd, f_hMainWndDC);

				PostQuitMessage(0);
			}
			else DefWindowProc(hWnd, msg, wParam, lParam);
			break;
		}

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return 0;
	}
	VOID Engine::EngineLoop()
	{
		OnCreate();

		while (f_bAtomActive)
		{
			//Calculating time between updates
			f_timePoint2 = std::chrono::system_clock::now();
			std::chrono::duration<float> deltaTime = f_timePoint2 - f_timePoint1;
			f_timePoint1 = f_timePoint2;

			//Get mouse position
			POINT mousePos;
			GetCursorPos(&mousePos);
			ScreenToClient(f_hMainWnd, &mousePos);
			f_iMousePosX = mousePos.x;
			f_iMousePosY = mousePos.y;

			//Keys info update
			for (int i = 0; i < 256; i++)
			{
				f_keysNewState[i] = GetKeyState(i);

				f_keys[i].KeyDown = false;
				f_keys[i].KeyUp = false;

				if (f_keysNewState[i] != f_keysOldState[i])
				{
					if (f_keysNewState[i] & 0x8000)
					{
						f_keys[i].KeyDown = !f_keys[i].KeyPressed;
						f_keys[i].KeyPressed = true;
					}
					else
					{
						f_keys[i].KeyUp = true;
						f_keys[i].KeyPressed = false;
					}
				}

				f_keysOldState[i] = f_keysNewState[i];
			}

			//Update
			OnUpdate(deltaTime.count());

			//Fps show
			if (f_bShowFps)
			{
				TCHAR fpsbuf[256];
				TCHAR mouseposbuf[256];
				ZeroMemory(fpsbuf, 256);
				ZeroMemory(mouseposbuf, 256);
				swprintf_s(fpsbuf, 256, L"FPS:%3.0f", 1.0f / deltaTime.count());
				swprintf_s(mouseposbuf, 256, L"MOUSE:X=%d Y=%d", GetMouseX(), GetMouseY());
				RECT textRect = { 0,0,lstrlen(fpsbuf) * 10,25 };
				RECT textRect2 = { 0,20,lstrlen(mouseposbuf) * 10,45 };
				DrawText(f_hBufferDC, fpsbuf, lstrlen(fpsbuf), &textRect, NULL);
				DrawText(f_hBufferDC, mouseposbuf, lstrlen(mouseposbuf), &textRect2, DT_BOTTOM);
			}

			for (GUI::UIElement* el : f_vuiElements)
			{
				bool isMouseHoverElement = false;

				Events::EventArgs args;
				args.engine = this;

				if (GetMouseX() <= el->GetX() + el->GetWidth() && GetMouseX() >= el->GetX())
				{
					if (GetMouseY() <= el->GetY() + el->GetHeight() && GetMouseY() >= el->GetY())
					{
						isMouseHoverElement = true;
					}
				}

				if (isMouseHoverElement)
				{
					el->SetMouseHover(true);
					el->onMouseHover(el, &args);
					if (GetKeyInfo(KeyCode::MOUSELEFT)->KeyDown)
					{
						el->onMouseDown(el, &args);
					}
					else if (GetKeyInfo(KeyCode::MOUSELEFT)->KeyUp) el->onMouseUp(el, &args);
					else if (GetKeyInfo(KeyCode::MOUSELEFT)->KeyPressed) el->onMousePressed(el, &args);
				}
				else if (el->IsMouseHover()) el->SetMouseHover(false);

				el->UpdateElement(this);
			}

			//Copying from buffer to screen
			f_mutexBuffer.lock();
			StretchBlt
			(
				f_hMainWndDC,
				0,
				0,
				GetClientWidth(),
				GetClientHeight(),
				f_hBufferDC,
				0,
				0,
				GetClientWidth(),
				GetClientHeight(),
				SRCCOPY
			);
			f_mutexBuffer.unlock();

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		OnDestroy();

		f_cvEngineLoopFinished.notify_one();
	}

	KeyInfo::KeyInfo() : KeyDown(false), KeyUp(false), KeyPressed(false) {}
}
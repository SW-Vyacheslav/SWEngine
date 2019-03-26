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
		f_hCurrentBrush = CreateSolidBrush(Color::Colors::BLACK.ToCOLORREF());
		f_hCurrentPen = CreatePen(PenStyle::SOLID, 1, Color::Colors::BLACK.ToCOLORREF());
		f_cCurrentBrushColor = Color::Colors::BLACK.ToCOLORREF();
		f_cCurrentPenColor = Color::Colors::BLACK.ToCOLORREF();
		f_iCurrentPenWidth = 1;
		f_iCurrentPenStyle = PenStyle::SOLID;
		SelectObject(f_hBufferDC, f_hBufferBitmap);
		SelectObject(f_hBufferDC, f_hCurrentBrush);
		SelectObject(f_hBufferDC, f_hCurrentPen);
		SetStretchBltMode(f_hMainWndDC, COLORONCOLOR);
		SetStretchBltMode(f_hBufferDC, COLORONCOLOR);

		ShowWindow(f_hMainWnd, SW_SHOW);
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		UpdateWindow(f_hMainWnd);

		f_bAtomActive = true;

		MSG msg;
		std::thread engine_thread(&Engine::EngineLoop, this);

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
				x++;
				y += incy;
				d += 2 * dy - 2 * dx;
			}
			else
			{
				x++;
				d += 2 * dy;
			}
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
		int miny = min(y1, min(y2, y3));
		int maxy = max(y1, max(y2, y3));
		int minx = min(x1, min(x2, x3));
		int maxx = max(x1, max(x2, x3));

		ScanLine scanline = { minx,miny,maxx,miny };

		ScanLine AB = { x1,y1,x2,y2 };
		ScanLine BC = { x2,y2,x3,y3 };
		ScanLine AC = { x1,y1,x3,y3 };

		POINT crosspoint1;
		POINT crosspoint2;

		for (int y = miny; y <= maxy; y += 1)
		{
			scanline.GetCrossPoint(AB, crosspoint1);
			scanline.GetCrossPoint(BC, crosspoint2);

			DrawLine(crosspoint1.x, crosspoint1.y, crosspoint2.x, crosspoint2.y, color);

			scanline.SetY1(y + 1);
			scanline.SetY2(y + 1);
		}
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
		SetCurrentPenStyle(PenStyle::SOLID);
		SetCurrentPenColor(color);
		SetCurrentPenWidth(1);
		SetCurrentBrushColor(color);

		Ellipse(f_hBufferDC, cx - radius, cy - radius, cx + radius, cy + radius);
	}
	VOID Engine::DrawPixelWinAPI(const int& x, const int& y, const COLORREF& color)
	{
		SetPixel(f_hBufferDC, x, y, color);
	}
	VOID Engine::DrawLineWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const COLORREF& color, const UINT& width, const INT& pen_style)
	{
		SetCurrentPenColor(color);
		SetCurrentPenWidth(width);
		SetCurrentPenStyle(pen_style);

		MoveToEx(f_hBufferDC, x1, y1, NULL);
		LineTo(f_hBufferDC, x2, y2);
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
		DrawText(f_hBufferDC, str, lstrlenW(str), &textRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		SetBkMode(f_hBufferDC, prevmode);
		SetTextColor(f_hBufferDC, prevcolor);
	}
	VOID Engine::DrawImageWinAPI(const HBITMAP& bitmap, const int& x1, const int& y1, const UINT& width, const UINT& height)
	{
		HDC hDC = CreateCompatibleDC(f_hMainWndDC);
		SelectObject(hDC, bitmap);
		BITMAP bm;
		GetObject(bitmap, sizeof(bm), &bm);
		if (width != 0 && height != 0) StretchBlt(f_hBufferDC, x1, y1, width, height, hDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		else StretchBlt(f_hBufferDC, x1, y1, bm.bmWidth, bm.bmHeight, hDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		DeleteDC(hDC);
	}
	VOID Engine::FillTriangleWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const COLORREF& color)
	{
		SetCurrentBrushColor(color);
		SetCurrentPenStyle(PenStyle::SOLID);
		SetCurrentPenColor(color);
		SetCurrentPenWidth(1);

		POINT p[3];
		p[0].x = x1;
		p[0].y = y1;
		p[1].x = x2;
		p[1].y = y2;
		p[2].x = x3;
		p[2].y = y3;
		Polygon(f_hBufferDC, p, 3);
	}
	VOID Engine::FillRectangleWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const COLORREF& color)
	{
		SetCurrentBrushColor(color);
		SetCurrentPenColor(color);
		SetCurrentPenWidth(1);
		SetCurrentPenStyle(PenStyle::SOLID);

		Rectangle(f_hBufferDC, x1, y1, x2, y2);
	}
	VOID Engine::FillWindowWinAPI(const COLORREF& color)
	{
		SetCurrentBrushColor(color);
		SetCurrentPenStyle(PenStyle::SOLID);
		SetCurrentPenColor(color);

		Rectangle(f_hBufferDC, 0, 0, GetClientWidth(), GetClientHeight());
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
				DeleteObject(f_hCurrentBrush);
				DeleteObject(f_hCurrentPen);
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
	VOID Engine::SetCurrentBrushColor(const COLORREF& color)
	{
		if (f_cCurrentBrushColor != color)
		{
			if (f_hCurrentBrush) DeleteObject(f_hCurrentBrush);
			f_hCurrentBrush = CreateSolidBrush(color);
			f_cCurrentBrushColor = color;
			SelectObject(f_hBufferDC, f_hCurrentBrush);
		}
	}
	VOID Engine::SetCurrentPenColor(const COLORREF& color)
	{
		if (f_cCurrentPenColor != color)
		{
			if (f_hCurrentPen) DeleteObject(f_hCurrentPen);
			f_hCurrentPen = CreatePen(PS_SOLID, f_iCurrentPenWidth, color);
			f_cCurrentPenColor = color;
			SelectObject(f_hBufferDC, f_hCurrentPen);
		}
	}
	VOID Engine::SetCurrentPenWidth(const INT& width)
	{
		if (f_iCurrentPenWidth != width)
		{
			if (f_hCurrentPen) DeleteObject(f_hCurrentPen);
			f_hCurrentPen = CreatePen(PS_SOLID, width, f_cCurrentPenColor);
			f_iCurrentPenWidth = width;
			SelectObject(f_hBufferDC, f_hCurrentPen);
		}
	}
	VOID Engine::SetCurrentPenStyle(const INT& style)
	{
		if (f_iCurrentPenStyle != style)
		{
			if (f_hCurrentPen) DeleteObject(f_hCurrentPen);
			f_hCurrentPen = CreatePen(style, f_iCurrentPenWidth, f_cCurrentPenColor);
			f_iCurrentPenStyle = style;
			SelectObject(f_hBufferDC, f_hCurrentPen);
		}
	}

	KeyInfo::KeyInfo() : KeyDown(false), KeyUp(false), KeyPressed(false) {}

	ScanLine::ScanLine() : x1(0), y1(0), x2(0), y2(0) {}
	ScanLine::ScanLine(const int& x1, const int& y1, const int& x2, const int& y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}
	void ScanLine::SetY1(const int& y1)
	{
		this->y1 = y1;
	}
	void ScanLine::SetY2(const int& y2)
	{
		this->y2 = y2;
	}
	bool ScanLine::GetCrossPoint(const ScanLine& line, POINT& outpoint)
	{
		int znam = (x1 - x2)*(line.y1 - line.y2) - (y1 - y2)*(line.x1 - line.x2);

		if (znam == 0) return false;

		outpoint.x = ((x1*y2 - y1 * x2)*(line.x1 - line.x2) - (x1 - x2)*(line.x1*line.y2 - line.y1*line.x2)) / znam;
		outpoint.y = ((x1*y2 - y1 * x2)*(line.y1 - line.y2) - (y1 - y2)*(line.x1*line.y2 - line.y1*line.x2)) / znam;

		return true;
	}
}
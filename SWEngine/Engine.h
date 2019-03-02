#pragma once

#ifndef UNICODE
#error UNICODE is not enabled for your compiler!
#endif

#include <Windows.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <fstream>
#include <strstream>
#include <algorithm>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <gdiplus.h>
#include <objidl.h>

#pragma comment (lib,"Gdiplus.lib")

static struct KeyCode
{
	static const INT A = 0x41;
	static const INT B = 0x42;
	static const INT C = 0x43;
	static const INT D = 0x44;
	static const INT E = 0x45;
	static const INT F = 0x46;
	static const INT G = 0x47;
	static const INT H = 0x48;
	static const INT I = 0x49;
	static const INT J = 0x4A;
	static const INT K = 0x4B;
	static const INT L = 0x4C;
	static const INT M = 0x4D;
	static const INT N = 0x4E;
	static const INT O = 0x4F;
	static const INT P = 0x50;
	static const INT Q = 0x51;
	static const INT R = 0x52;
	static const INT S = 0x53;
	static const INT T = 0x54;
	static const INT U = 0x55;
	static const INT V = 0x56;
	static const INT W = 0x57;
	static const INT X = 0x58;
	static const INT Y = 0x59;
	static const INT Z = 0x5A;
	static const INT MOUSELEFT = 0x01;
	static const INT MOUSERIGHT = 0x02;
	static const INT MOUSEMIDDLE = 0x04;
	static const INT BACKSPACE = 0x08;
	static const INT TAB = 0x09;
	static const INT ENTER = 0x0D;
	static const INT SHIFT = 0x10;
	static const INT CTRL = 0x11;
	static const INT ALT = 0x12;
	static const INT ESC = 0x1B;
	static const INT SPACE = 0x20;
	static const INT LEFTARROW = 0x25;
	static const INT UPARROW = 0x26;
	static const INT RIGHTARROW = 0x27;
	static const INT DOWNARROW = 0x28;
};
struct KeyInfo
{
	bool KeyDown;
	bool KeyUp;
	bool KeyPressed;
};
static struct Color
{
	static const COLORREF RED = RGB(255, 0, 0);
	static const COLORREF GREEN = RGB(0, 255, 0);
	static const COLORREF BLUE = RGB(0, 0, 255);
	static const COLORREF BLACK = RGB(0, 0, 0);
	static const COLORREF WHITE = RGB(255, 255, 255);
};
static struct PenStyle
{
	static const INT SOLID = PS_SOLID;
	static const INT EMPTY = PS_NULL;
	static const INT DASH = PS_DASH;
	static const INT DOT = PS_DOT;
	static const INT DASHDOT = PS_DASHDOT;
	static const INT DASHDOTDOT = PS_DASHDOTDOT;
	static const INT INSIDEFRAME = PS_INSIDEFRAME;
};

/*
struct Line
{
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;

	bool GetCrossPoint(const Line& line, POINT& outpoint)
	{
		float znam = (x1 - x2)*(line.y1 - line.y2) - (y1 - y2)*(line.x1 - line.x2);

		if (znam == 0) return false;

		outpoint.x = ((x1*y2 - y1 * x2)*(line.x1 - line.x2) - (x1 - x2)*(line.x1*line.y2 - line.y1*line.x2)) / znam;
		outpoint.y = ((x1*y2 - y1 * x2)*(line.y1 - line.y2) - (y1 - y2)*(line.x1*line.y2 - line.y1*line.x2)) / znam;

		return true;
	}
};
*/

class Engine
{
public:
	explicit Engine()
	{
		f_bShowFps = false;
		f_bAtomActive = false;
	}

	VOID Start(UINT windowWidth, UINT windowHeight)
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
		f_hCurrentBrush = CreateSolidBrush(Color::BLACK);
		f_hCurrentPen = CreatePen(PenStyle::SOLID, 1, Color::BLACK);
		f_cCurrentBrushColor = Color::BLACK;
		f_cCurrentPenColor = Color::BLACK;
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

	UINT GetClientWidth() const
	{
		return f_uiClientWidth;
	}

	UINT GetClientHeight() const
	{
		return f_uiClientHeight;
	}

	INT GetMouseX() const
	{
		return f_iMousePosX;
	}

	INT GetMouseY() const
	{
		return f_iMousePosY;
	}

	VOID ShowFps(bool isShowed)
	{
		f_bShowFps = isShowed;
	}

	bool IsFpsShowed() const
	{
		return f_bShowFps;
	}

	const KeyInfo* GetKeyInfo(const INT& keycode) const
	{
		if (keycode < 0 || keycode > 255) return NULL;

		return &f_keys[keycode];
	}

protected:
	virtual VOID OnCreate() {};
	virtual VOID OnUpdate(float fdeltaTime) {};
	virtual VOID OnDestroy() {}

	/*
	VOID DrawLine(const int& x1, const int& y1, const int& x2, const int& y2, const COLORREF& color)
	{
		if (x1 == x2)
		{
			for (int y = y1; y <= y2; y++)
			{
				DrawPixel(x1, y, color);
			}
		}
		else
		{
			for (int x = x1; x <= x2; x++)
			{
				int y = (((x - x1)*(y2 - y1)) / (x2 - x1)) + y1;
				DrawPixel(x, y, color);
			}
		}
	}

	VOID DrawTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const COLORREF& color)
	{
		DrawLine(x1, y1, x2, y2, color);
		DrawLine(x2, y2, x3, y3, color);
		DrawLine(x3, y3, x1, y1, color);
	}

	VOID FillTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const COLORREF& color)
	{
		int miny = min(y1, min(y2, y3));
		int maxy = max(y1, max(y2, y3));
		int minx = min(x1, min(x2, x3));
		int maxx = max(x1, max(x2, x3));

		Line scanline = { minx,miny,maxx,miny };

		Line AB = { x1,y1,x2,y2 };
		Line BC = { x2,y2,x3,y3 };
		Line AC = { x1,y1,x3,y3 };

		POINT crosspoint1;
		POINT crosspoint2;

		for (int y = miny; y <= maxy; y+=1)
		{
			scanline.GetCrossPoint(AB,crosspoint1);
			scanline.GetCrossPoint(BC,crosspoint2);

			DrawLine(crosspoint1.x,crosspoint1.y, crosspoint2.x, crosspoint2.y, color);

			scanline.y1 = scanline.y2 = y + 1;
		}
	}
	*/

	VOID DrawCircle(const int& cx, const int& cy, const int& radius, const COLORREF& color)
	{
		for (int x = cx - radius; x <= cx + radius; x++)
		{
			int y = (int)sqrtf((radius*radius) - ((x - cx)*(x - cx))) + cy;
			int y2 = -y + 2 * cy;
			DrawPixel(x, y, color);
			DrawPixel(x, y2, color);
		}
	}

	VOID DrawPixel(const int& x, const int& y, const COLORREF& color)
	{
		SetPixel(f_hBufferDC, x, y, color);
	}

	VOID DrawLine(const int& x1, const int& y1, const int& x2, const int& y2, const COLORREF& color, const UINT& width = 1, const INT& pen_style = PenStyle::SOLID)
	{
		SetCurrentPenColor(color);
		SetCurrentPenWidth(width);
		SetCurrentPenStyle(pen_style);

		MoveToEx(f_hBufferDC, x1, y1, NULL);
		LineTo(f_hBufferDC, x2, y2);
	}

	VOID DrawTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const COLORREF& color, const UINT& border_width = 1, const INT& pen_style = PenStyle::SOLID)
	{
		DrawLine(x1, y1, x2, y2, color, border_width, pen_style);
		DrawLine(x2, y2, x3, y3, color, border_width, pen_style);
		DrawLine(x3, y3, x1, y1, color, border_width, pen_style);
	}

	VOID DrawImage(const HBITMAP& bitmap, const int& x1, const int& y1, const UINT& width = 0, const UINT& height = 0)
	{
		HDC hDC = CreateCompatibleDC(f_hMainWndDC);
		SelectObject(hDC, bitmap);
		BITMAP bm;
		GetObject(bitmap, sizeof(bm), &bm);
		if (width != 0 && height != 0) StretchBlt(f_hBufferDC, x1, y1, width, height, hDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		else StretchBlt(f_hBufferDC, x1, y1, bm.bmWidth, bm.bmHeight, hDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		DeleteDC(hDC);
	}

	HBITMAP LoadHBitmap(const LPCWSTR filename)
	{
		HBITMAP hbm;
		Gdiplus::Bitmap bm(filename);
		bm.GetHBITMAP(Gdiplus::Color(0, 0, 0), &hbm);

		return hbm;
	}

	VOID FillTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const COLORREF& color)
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

	VOID FillWindow(const COLORREF& color)
	{
		SetCurrentBrushColor(color);
		SetCurrentPenStyle(PenStyle::SOLID);
		SetCurrentPenColor(color);

		Rectangle(f_hBufferDC, 0, 0, GetClientWidth(), GetClientHeight());
	}

	VOID FillWindow(const HBITMAP& bitmap)
	{
		DrawImage(bitmap, 0, 0, GetClientWidth(), GetClientHeight());
	}

private:
	static LRESULT CALLBACK StaticMainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

	LRESULT MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

		case WM_MOUSEMOVE:
		{
			f_iMousePosX = LOWORD(lParam);
			f_iMousePosY = HIWORD(lParam);

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

	VOID EngineLoop()
	{
		OnCreate();

		while (f_bAtomActive)
		{
			//Calculating time between updates
			f_timePoint2 = std::chrono::system_clock::now();
			std::chrono::duration<float> deltaTime = f_timePoint2 - f_timePoint1;
			f_timePoint1 = f_timePoint2;

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

			f_mutexBuffer.lock();
			//Copying from buffer to screen
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

	VOID SetCurrentBrushColor(const COLORREF& color)
	{
		if (f_cCurrentBrushColor != color)
		{
			if (f_hCurrentBrush) DeleteObject(f_hCurrentBrush);
			f_hCurrentBrush = CreateSolidBrush(color);
			f_cCurrentBrushColor = color;
			SelectObject(f_hBufferDC, f_hCurrentBrush);
		}
	}

	VOID SetCurrentPenColor(const COLORREF& color)
	{
		if (f_cCurrentPenColor != color)
		{
			if (f_hCurrentPen) DeleteObject(f_hCurrentPen);
			f_hCurrentPen = CreatePen(PS_SOLID, f_iCurrentPenWidth, color);
			f_cCurrentPenColor = color;
			SelectObject(f_hBufferDC, f_hCurrentPen);
		}
	}

	VOID SetCurrentPenWidth(const INT& width)
	{
		if (f_iCurrentPenWidth != width)
		{
			if (f_hCurrentPen) DeleteObject(f_hCurrentPen);
			f_hCurrentPen = CreatePen(PS_SOLID, width, f_cCurrentPenColor);
			f_iCurrentPenWidth = width;
			SelectObject(f_hBufferDC, f_hCurrentPen);
		}
	}

	VOID SetCurrentPenStyle(const INT& style)
	{
		if (f_iCurrentPenStyle != style)
		{
			if (f_hCurrentPen) DeleteObject(f_hCurrentPen);
			f_hCurrentPen = CreatePen(style, f_iCurrentPenWidth, f_cCurrentPenColor);
			f_iCurrentPenStyle = style;
			SelectObject(f_hBufferDC, f_hCurrentPen);
		}
	}

private:
	HINSTANCE f_hInstance;
	HWND f_hMainWnd;
	HDC f_hMainWndDC;
	HDC f_hBufferDC;
	HBITMAP f_hBufferBitmap;

	HBRUSH f_hCurrentBrush;
	COLORREF f_cCurrentBrushColor;

	HPEN f_hCurrentPen;
	COLORREF f_cCurrentPenColor;
	INT f_iCurrentPenWidth;
	INT f_iCurrentPenStyle;

	UINT f_uiClientWidth;
	UINT f_uiClientHeight;
	std::chrono::system_clock::time_point f_timePoint1;
	std::chrono::system_clock::time_point f_timePoint2;

	KeyInfo f_keys[256];
	SHORT f_keysNewState[256];
	SHORT f_keysOldState[256];

	INT f_iMousePosX;
	INT f_iMousePosY;

	std::atomic<bool> f_bAtomActive;
	std::mutex f_mutexEngineLoop;
	std::mutex f_mutexBuffer;
	std::condition_variable f_cvEngineLoopFinished;

private:
	bool f_bShowFps;

};
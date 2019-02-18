#pragma once
#include <Windows.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <fstream>
#include <strstream>
#include <algorithm>

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
	bool KeyDown = false;
	bool KeyUp = false;
	bool KeyPressed = false;
};
static struct Color
{
	static const COLORREF RED = RGB(255, 0, 0);
	static const COLORREF GREEN = RGB(0, 255, 0);
	static const COLORREF BLUE = RGB(0, 0, 255);
	static const COLORREF BLACK = RGB(0, 0, 0);
	static const COLORREF WHITE = RGB(255, 255, 255);
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
	}

	VOID Start(UINT windowWidth, UINT windowHeight)
	{
		f_hInstance = GetModuleHandle(0);

		if (!f_hInstance) throw "Can't get an instance.";

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
		f_hCurrentPen = CreatePen(PS_NULL, NULL, Color::BLACK);
		SelectObject(f_hBufferDC, f_hBufferBitmap);
		SelectObject(f_hBufferDC, f_hCurrentBrush);
		SelectObject(f_hBufferDC, f_hCurrentPen);
		SetStretchBltMode(f_hMainWndDC, COLORONCOLOR);

		OnCreate();

		if (!f_hMainWnd) throw "Window is not created.";

		ShowWindow(f_hMainWnd, SW_SHOW);
		UpdateWindow(f_hMainWnd);
		SetTimer(f_hMainWnd, NULL, 10, NULL);

		MSG msg;

		while (GetMessage(&msg, NULL, NULL, NULL))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UINT GetClientWidth()
	{
		return f_uiClientWidth;
	}

	UINT GetClientHeight()
	{
		return f_uiClientHeight;
	}

	VOID ShowFps(bool isShowed)
	{
		f_bShowFps = isShowed;
	}

	bool IsFpsShowed()
	{
		return f_bShowFps;
	}

	const KeyInfo* GetKeyInfo(const INT& keycode)
	{
		if (keycode < 0 || keycode > 255) return NULL;

		return &f_keys[keycode];
	}

protected:
	virtual VOID OnCreate() = 0;
	virtual VOID OnUpdate(float fdeltaTime) = 0;
	virtual VOID OnDestroy() {}

	/*
	VOID DrawPixel(const int& x, const int& y, const COLORREF& color)
	{
		SetPixel(f_hBufferDC, x, y, color);
	}

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

	VOID DrawCircle(const int& cx,const int& cy,const int& radius, const COLORREF& color)
	{
		for (int x = cx - radius; x <= cx + radius; x++)
		{
			int y = (int)sqrtf((radius*radius) - ((x - cx)*(x - cx))) + cy;
			int y2 = -y + 2 * cy;
			DrawPixel(x, y, color);
			DrawPixel(x, y2, color);
		}
	}
	*/

	VOID DrawLine(const int& x1, const int& y1, const int& x2, const int& y2, const COLORREF& color)
	{
		SetCurrentPenColor(color);

		MoveToEx(f_hBufferDC, x1, y1, NULL);
		LineTo(f_hBufferDC, x2, y2);
	}

	VOID DrawTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const COLORREF& color)
	{
		SetCurrentPenColor(color);

		POINT p[3];
		p[0].x = x1;
		p[0].y = y1;
		p[1].x = x2;
		p[1].y = y2;
		p[2].x = x3;
		p[2].y = y3;
		Polygon(f_hBufferDC, p, 3);
	}

	VOID FillTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const COLORREF& color)
	{
		SetCurrentBrushColor(color);

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
		Rectangle(f_hBufferDC, 0, 0, GetClientWidth(), GetClientHeight());
	}

	VOID RefreshColors()
	{
		SetCurrentBrushColor(Color::BLACK);
		SetCurrentPenColor(Color::BLACK);
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
		case WM_TIMER:
		{
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

			RefreshColors();

			OnUpdate(deltaTime.count());

			if (f_bShowFps)
			{
				TCHAR buf[8];
				ZeroMemory(buf, 8);
				swprintf_s(buf, 8, L"FPS:%3.0f", 1.0f / deltaTime.count());
				RECT textRect = { 5,5,65,25 };
				DrawText(f_hBufferDC, buf, 8, &textRect, DT_LEFT);
			}

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
			break;
		}

		case WM_SIZE:
		{
			RECT clientRect;
			GetClientRect(f_hMainWnd, &clientRect);

			f_uiClientWidth = clientRect.right;
			f_uiClientHeight = clientRect.bottom;

			DeleteObject(f_hBufferBitmap);
			f_hBufferBitmap = CreateCompatibleBitmap(f_hMainWndDC, GetClientWidth(), GetClientHeight());
			SelectObject(f_hBufferDC, f_hBufferBitmap);

			break;
		}

		case WM_DESTROY:
		{
			DeleteObject(f_hBufferBitmap);
			DeleteObject(f_hBufferDC);
			DeleteObject(f_hCurrentBrush);
			DeleteObject(f_hCurrentPen);
			ReleaseDC(f_hMainWnd, f_hMainWndDC);
			OnDestroy();
			PostQuitMessage(0);
			break;
		}

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return 0;
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
			f_hCurrentPen = CreatePen(PS_SOLID, 1, color);
			f_cCurrentPenColor = color;
			SelectObject(f_hBufferDC, f_hCurrentPen);
		}
	}

private:
	HINSTANCE f_hInstance;
	HDC f_hMainWndDC;
	HDC f_hBufferDC;
	HBITMAP f_hBufferBitmap;
	HBRUSH f_hCurrentBrush;
	HPEN f_hCurrentPen;
	COLORREF f_cCurrentBrushColor;
	COLORREF f_cCurrentPenColor;
	HWND f_hMainWnd;
	UINT f_uiClientWidth;
	UINT f_uiClientHeight;
	std::chrono::system_clock::time_point f_timePoint1;
	std::chrono::system_clock::time_point f_timePoint2;
	KeyInfo f_keys[256];
	SHORT f_keysNewState[256];
	SHORT f_keysOldState[256];

private:
	bool f_bShowFps;

};
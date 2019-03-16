#pragma once

#ifndef UNICODE
#error UNICODE is not enabled for your compiler!
#endif

#include <Windows.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <gdiplus.h>
#include <objidl.h>
#include <vector>
#include <list>

#pragma comment (lib,"Gdiplus.lib")

class ScanLine
{
public:
	ScanLine() : x1(0), y1(0), x2(0), y2(0) {}
	ScanLine(const int& x1, const int& y1, const int& x2, const int& y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}

	void SetY1(const int& y1)
	{
		this->y1 = y1;
	}
	void SetY2(const int& y2)
	{
		this->y2 = y2;
	}

	bool GetCrossPoint(const ScanLine& line, POINT& outpoint)
	{
		int znam = (x1 - x2)*(line.y1 - line.y2) - (y1 - y2)*(line.x1 - line.x2);

		if (znam == 0) return false;

		outpoint.x = ((x1*y2 - y1 * x2)*(line.x1 - line.x2) - (x1 - x2)*(line.x1*line.y2 - line.y1*line.x2)) / znam;
		outpoint.y = ((x1*y2 - y1 * x2)*(line.y1 - line.y2) - (y1 - y2)*(line.x1*line.y2 - line.y1*line.x2)) / znam;

		return true;
	}

private:
	int x1;
	int y1;
	int x2;
	int y2;
};

class KeyInfo
{
public:
	KeyInfo() : KeyDown(false), KeyUp(false), KeyPressed(false) {}

	bool KeyDown;
	bool KeyUp;
	bool KeyPressed;
};

class RGBValue
{
public:
	RGBValue() : r(0), g(0), b(0) {}
	RGBValue(const int& r, const int& g, const int& b)
	{
		if ((r >= 0 && r <= 255) && (g >= 0 && g <= 255) && (b >= 0 && b <= 255))
		{
			this->r = r;
			this->g = g;
			this->b = b;
		}
		else
		{
			this->r = 0;
			this->g = 0;
			this->b = 0;
		}
	}

	const int& GetR() const
	{
		return r;
	}
	const int& GetG() const
	{
		return g;
	}
	const int& GetB() const
	{
		return b;
	}
	void SetR(const int& r)
	{
		if (r >= 0 && r <= 255) this->r = r;
	}
	void SetG(const int& g)
	{
		if (g >= 0 && g <= 255) this->g = g;
	}
	void SetB(const int& b)
	{
		if (b >= 0 && b <= 255) this->b = b;
	}
	void SetRGB(const int& r, const int& g, const int& b)
	{
		if ((r >= 0 && r <= 255) && (g >= 0 && g <= 255) && (b >= 0 && b <= 255))
		{
			this->r = r;
			this->g = g;
			this->b = b;
		}
	}

private:
	int r;
	int g;
	int b;
};
class HSLValue
{
public:
	HSLValue() : h(0), s(0), l(0) {}
	HSLValue(const int& h, const int& s, const int& l)
	{
		if ((h >= 0 && h <= 360) && (s >= 0 && s <= 100) && (l >= 0 && l <= 100))
		{
			this->h = h;
			this->s = s;
			this->l = l;
		}
		else
		{
			this->h = 0;
			this->s = 0;
			this->l = 0;
		}
	}

	const int& GetH() const
	{
		return h;
	}
	const int& GetS() const
	{
		return s;
	}
	const int& GetL() const
	{
		return l;
	}
	void SetH(const int& h)
	{
		if (h >= 0 && h <= 360) this->h = h;
	}
	void SetS(const int& s)
	{
		if (s >= 0 && s <= 100) this->s = s;
	}
	void SetL(const int& l)
	{
		if (l >= 0 && l <= 100) this->l = l;
	}
	void SetHSL(const int& h, const int& s, const int& l)
	{
		if ((h >= 0 && h <= 360) && (s >= 0 && s <= 100) && (l >= 0 && l <= 100))
		{
			this->h = h;
			this->s = s;
			this->l = l;
		}
	}

private:
	int h;
	int s;
	int l;
};

class IColorFormat abstract
{
public:
	virtual void ToRGBColorFormat(int& r, int& g, int& b) const = 0;
	virtual void ToRGBColorFormat(RGBValue& val) const = 0;
	virtual void ToHSLColorFormat(int& h, int& s, int& l) const = 0;
	virtual void ToHSLColorFormat(HSLValue& val) const = 0;
	virtual void ToHEXColorFormat(std::string& hex) const = 0;
};
class RGBColorFormat : public IColorFormat, RGBValue
{
public:
	RGBColorFormat() : RGBValue() {}
	RGBColorFormat(const int& r, const int& g, const int& b) : RGBValue(r,g,b) {}

	void ToRGBColorFormat(int& r, int& g, int& b) const override
	{
		r = GetR();
		g = GetG();
		b = GetB();
	}
	void ToRGBColorFormat(RGBValue& val) const override
	{
		val.SetR(GetR());
		val.SetG(GetG());
		val.SetB(GetB());
	}
	void ToHSLColorFormat(int& h, int& s, int& l) const override
	{

	}
	void ToHSLColorFormat(HSLValue& val) const override
	{

	}
	void ToHEXColorFormat(std::string& hex) const override
	{

	}
};
class HSLColorFormat : public IColorFormat, HSLValue
{
public:
	HSLColorFormat() : HSLValue() {}
	HSLColorFormat(const int& h, const int& s, const int& l) : HSLValue(h, s, l) {}

	void ToRGBColorFormat(int& r, int& g, int& b) const override
	{
		float c = (1.0f - std::abs(2.0f * ((float)GetL() / 100.0f) - 1.0f)) * ((float)GetS() / 100.0f);
		float x = c * (1.0f - (float)std::abs((GetH() / 60) % 2 - 1));
		float m = ((float)GetL() / 100.0f) - ((float)c / 2.0f);

		if (GetH() >= 0 && GetH() < 60)
		{
			r = (int)c;
			g = (int)x;
			b = 0;
		}
		else if (GetH() >= 60 && GetH() < 120)
		{
			r = (int)x;
			g = (int)c;
			b = 0;
		}
		else if (GetH() >= 120 && GetH() < 180)
		{
			r = 0;
			g = (int)c;
			b = (int)x;
		}
		else if (GetH() >= 180 && GetH() < 240)
		{
			r = 0;
			g = (int)x;
			b = (int)c;
		}
		else if (GetH() >= 240 && GetH() < 300)
		{
			r = (int)x;
			g = 0;
			b = (int)c;
		}
		else if (GetH() >= 300 && GetH() < 360)
		{
			r = (int)c;
			g = 0;
			b = (int)x;
		}

		r = (int)(((float)r + m) * 255.0f);
		g = (int)(((float)g + m) * 255.0f);
		b = (int)(((float)b + m) * 255.0f);
	}
	void ToRGBColorFormat(RGBValue& val) const override
	{
		int r = 0;
		int g = 0;
		int b = 0;
		ToRGBColorFormat(r, g, b);
		val.SetR(r);
		val.SetG(g);
		val.SetB(b);
	}
	void ToHSLColorFormat(int& h, int& s, int& l) const override
	{
		h = GetH();
		s = GetS();
		l = GetL();
	}
	void ToHSLColorFormat(HSLValue& val) const override
	{
		val.SetH(GetH());
		val.SetS(GetS());
		val.SetL(GetL());
	}
	void ToHEXColorFormat(std::string& hex) const override
	{

	}
};
class HEXColorFormat : public IColorFormat
{
public:
	HEXColorFormat() : hex("#000000") {}
	HEXColorFormat(const std::string& hex)
	{
		if (hex[0] == '#' && hex.size() == 7)
		{
			this->hex = hex;

			for (int i = 1; i < 7; i++)
			{
				if ((hex[i] >= '0' && hex[i] <= '9') || (hex[i] >= 'a' && hex[i] <= 'f')) continue;
				else
				{
					this->hex = "#000000";
					break;
				}
			}
		}
	}

	void ToRGBColorFormat(int& r, int& g, int& b) const override
	{
		int num = 0;

		if (hex[1] >= 'a' && hex[1] <= 'f') num = hex[1] - 87;
		else num = atoi(&hex[1]);
		r = r | num << 4;
		if (hex[2] >= 'a' && hex[2] <= 'f') num = hex[2] - 87;
		else num = atoi(&hex[2]);
		r = r | num;

		if (hex[3] >= 'a' && hex[3] <= 'f') num = hex[3] - 87;
		else num = atoi(&hex[3]);
		g = g | num << 4;
		if (hex[4] >= 'a' && hex[4] <= 'f') num = hex[4] - 87;
		else num = atoi(&hex[4]);
		g = g | num;

		if (hex[5] >= 'a' && hex[5] <= 'f') num = hex[5] - 87;
		else num = atoi(&hex[5]);
		b = b | num << 4;
		if (hex[6] >= 'a' && hex[6] <= 'f') num = hex[6] - 87;
		else num = atoi(&hex[6]);
		b = b | num;
	}
	void ToRGBColorFormat(RGBValue& val) const override
	{
		int r = 0;
		int g = 0;
		int b = 0;
		ToRGBColorFormat(r, g, b);
		val.SetR(r);
		val.SetG(g);
		val.SetB(b);
	}
	void ToHSLColorFormat(int& h, int& s, int& l) const override
	{

	}
	void ToHSLColorFormat(HSLValue& val) const override
	{

	}
	void ToHEXColorFormat(std::string& hex) const override
	{
		hex = this->hex;
	}

private:
	std::string hex;
};

struct _KeyCode
{
	const INT A = 0x41;
	const INT B = 0x42;
	const INT C = 0x43;
	const INT D = 0x44;
	const INT E = 0x45;
	const INT F = 0x46;
	const INT G = 0x47;
	const INT H = 0x48;
	const INT I = 0x49;
	const INT J = 0x4A;
	const INT K = 0x4B;
	const INT L = 0x4C;
	const INT M = 0x4D;
	const INT N = 0x4E;
	const INT O = 0x4F;
	const INT P = 0x50;
	const INT Q = 0x51;
	const INT R = 0x52;
	const INT S = 0x53;
	const INT T = 0x54;
	const INT U = 0x55;
	const INT V = 0x56;
	const INT W = 0x57;
	const INT X = 0x58;
	const INT Y = 0x59;
	const INT Z = 0x5A;
	const INT MOUSELEFT = 0x01;
	const INT MOUSERIGHT = 0x02;
	const INT MOUSEMIDDLE = 0x04;
	const INT BACKSPACE = 0x08;
	const INT TAB = 0x09;
	const INT ENTER = 0x0D;
	const INT SHIFT = 0x10;
	const INT CTRL = 0x11;
	const INT ALT = 0x12;
	const INT ESC = 0x1B;
	const INT SPACE = 0x20;
	const INT LEFTARROW = 0x25;
	const INT UPARROW = 0x26;
	const INT RIGHTARROW = 0x27;
	const INT DOWNARROW = 0x28;
} KeyCode;
struct _Colors
{
	const RGBColorFormat RED = { 255,0,0 };
	const RGBColorFormat GREEN = { 0, 255, 0 };
	const RGBColorFormat BLUE = { 0, 0, 255 };
	const RGBColorFormat BLACK = { 0, 0, 0 };
	const RGBColorFormat WHITE = { 255, 255, 255 };
	const RGBColorFormat YELLOW = { 255, 255, 0 };
	const RGBColorFormat MAGENTA = { 255, 0, 255 };
	const RGBColorFormat AQUA = { 0, 255, 255 };
	const RGBColorFormat GREY = { 128, 128, 128 };
} Colors;
struct _PenStyle
{
	const INT SOLID = PS_SOLID;
	const INT EMPTY = PS_NULL;
	const INT DASH = PS_DASH;
	const INT DOT = PS_DOT;
	const INT DASHDOT = PS_DASHDOT;
	const INT DASHDOTDOT = PS_DASHDOTDOT;
	const INT INSIDEFRAME = PS_INSIDEFRAME;
} PenStyle;

class UIElement abstract
{
public:
	UIElement() : x(0), y(0), width(10), height(10), isFocused(false), backgroundColor(Colors.GREY), foregroundColor(Colors.BLACK) {}

	void SetX(const int& x)
	{
		this->x = x;
	}
	void SetY(const int& y)
	{
		this->y = y;
	}
	void SetWidth(const int& width)
	{
		this->width = width;
	}
	void SetHeigth(const int& height)
	{
		this->height = height;
	}
	void SetText(const std::wstring& text)
	{
		this->text = text;
	}

	const int& GetX() const
	{
		return x;
	}
	const int& GetY() const
	{
		return y;
	}
	const int& GetWidth() const
	{
		return width;
	}
	const int& GetHeight() const
	{
		return height;
	}
	const RGBColorFormat& GetBackgroundColor() const
	{
		return backgroundColor;
	}
	const RGBColorFormat& GetForegroundColor() const
	{
		return foregroundColor;
	}
	const std::wstring& GetText() const
	{
		return text;
	}

	virtual void Draw(void* eng) const = 0;

	const bool& IsFocused() const
	{
		return isFocused;
	}
	void SetFocus()
	{
		isFocused = true;
	}
	void UnsetFocus()
	{
		isFocused = false;
	}

	bool IsMouseHover(const float& mousex, const float& mousey) const
	{
		if (mousex <= x + width && mousex >= x)
		{
			if (mousey <= y + height && mousey >= y)
			{
				return true;
			}
		}

		return false;
	}

private:
	int x;
	int y;
	int width;
	int height;
	bool isFocused;
	std::wstring text;
	RGBColorFormat backgroundColor;
	RGBColorFormat foregroundColor;
};

class Engine
{
public:
	Engine() : f_bShowFps(false), f_bAtomActive(false) {}

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
		f_hCurrentBrush = CreateSolidBrush(ToCOLORREF(Colors.BLACK));
		f_hCurrentPen = CreatePen(PenStyle.SOLID, 1, ToCOLORREF(Colors.BLACK));
		f_cCurrentBrushColor = ToCOLORREF(Colors.BLACK);
		f_cCurrentPenColor = ToCOLORREF(Colors.BLACK);
		f_iCurrentPenWidth = 1;
		f_iCurrentPenStyle = PenStyle.SOLID;
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

	VOID AddUIElement(UIElement& el)
	{
		f_vuiElements.push_back(&el);
	}

	COLORREF ToCOLORREF(const IColorFormat& format)
	{
		RGBValue val;
		format.ToRGBColorFormat(val);
		return RGB(val.GetR(), val.GetG(), val.GetB());
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

	//SWEngine drawing methods
	VOID DrawLine(const int& x1, const int& y1, const int& x2, const int& y2, const IColorFormat& color)
	{
		const int deltaX = abs(x2 - x1);
		const int deltaY = abs(y2 - y1);
		const int signX = x1 < x2 ? 1 : -1;
		const int signY = y1 < y2 ? 1 : -1;
		int error = deltaX - deltaY;

		int r = 0;
		int g = 0;
		int b = 0;
		color.ToRGBColorFormat(r, g, b);
		DrawPixelWinAPI(x2, y2, ToCOLORREF(color));

		int x = x1;
		int y = y1;
		while (x != x2 || y != y2)
		{
			DrawPixelWinAPI(x, y, ToCOLORREF(color));

			const int error2 = error * 2;
			if (error2 > -deltaY)
			{
				error -= deltaY;
				x += signX;
			}
			if (error2 < deltaX)
			{
				error += deltaX;
				y += signY;
			}
		}
	}

	VOID DrawTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const IColorFormat& color)
	{
		DrawLine(x1, y1, x2, y2, color);
		DrawLine(x2, y2, x3, y3, color);
		DrawLine(x3, y3, x1, y1, color);
	}

	VOID FillTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const IColorFormat& color)
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

	VOID DrawCircle(const int& cx, const int& cy, const int& radius, const IColorFormat& color)
	{
		int x = 0;
		int y = radius;
		int delta = 1 - 2 * radius;
		int error = 0;

		while (y >= 0)
		{
			DrawPixelWinAPI(cx + x, cy + y, ToCOLORREF(color));
			DrawPixelWinAPI(cx + x, cy - y, ToCOLORREF(color));
			DrawPixelWinAPI(cx - x, cy + y, ToCOLORREF(color));
			DrawPixelWinAPI(cx - x, cy - y, ToCOLORREF(color));
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
	VOID FillCircleWinAPI(const int& cx, const int& cy, const int& radius, const COLORREF& color)
	{
		SetCurrentPenStyle(PenStyle.SOLID);
		SetCurrentPenColor(color);
		SetCurrentPenWidth(1);
		SetCurrentBrushColor(color);

		Ellipse(f_hBufferDC, cx - radius, cy - radius, cx + radius, cy + radius);
	}

	VOID DrawPixelWinAPI(const int& x, const int& y, const COLORREF& color)
	{
		SetPixel(f_hBufferDC, x, y, color);
	}

	VOID DrawLineWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const COLORREF& color, const UINT& width = 1, const INT& pen_style = PenStyle.SOLID)
	{
		SetCurrentPenColor(color);
		SetCurrentPenWidth(width);
		SetCurrentPenStyle(pen_style);

		MoveToEx(f_hBufferDC, x1, y1, NULL);
		LineTo(f_hBufferDC, x2, y2);
	}

	VOID DrawTriangleWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const COLORREF& color, const UINT& border_width = 1, const INT& pen_style = PenStyle.SOLID)
	{
		DrawLineWinAPI(x1, y1, x2, y2, color, border_width, pen_style);
		DrawLineWinAPI(x2, y2, x3, y3, color, border_width, pen_style);
		DrawLineWinAPI(x3, y3, x1, y1, color, border_width, pen_style);
	}

	VOID DrawStringWinAPI(LPCWSTR str, const int& x1, const int& y1, const int& x2, const int& y2, const COLORREF& textcolor)
	{
		RECT textRect = { x1,y1,x2,y2 };
		int prevmode = SetBkMode(f_hBufferDC, TRANSPARENT);
		COLORREF prevcolor = SetTextColor(f_hBufferDC, textcolor);
		DrawText(f_hBufferDC, str, lstrlenW(str), &textRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		SetBkMode(f_hBufferDC, prevmode);
		SetTextColor(f_hBufferDC, prevcolor);
	}

	VOID DrawImageWinAPI(const HBITMAP& bitmap, const int& x1, const int& y1, const UINT& width = 0, const UINT& height = 0)
	{
		HDC hDC = CreateCompatibleDC(f_hMainWndDC);
		SelectObject(hDC, bitmap);
		BITMAP bm;
		GetObject(bitmap, sizeof(bm), &bm);
		if (width != 0 && height != 0) StretchBlt(f_hBufferDC, x1, y1, width, height, hDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		else StretchBlt(f_hBufferDC, x1, y1, bm.bmWidth, bm.bmHeight, hDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		DeleteDC(hDC);
	}

	VOID FillTriangleWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const COLORREF& color)
	{
		SetCurrentBrushColor(color);
		SetCurrentPenStyle(PenStyle.SOLID);
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

	VOID FillRectangleWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const COLORREF& color)
	{
		SetCurrentBrushColor(color);
		SetCurrentPenColor(color);
		SetCurrentPenWidth(1);
		SetCurrentPenStyle(PenStyle.SOLID);

		Rectangle(f_hBufferDC, x1, y1, x2, y2);
	}

	VOID FillWindowWinAPI(const COLORREF& color)
	{
		SetCurrentBrushColor(color);
		SetCurrentPenStyle(PenStyle.SOLID);
		SetCurrentPenColor(color);

		Rectangle(f_hBufferDC, 0, 0, GetClientWidth(), GetClientHeight());
	}

	VOID FillWindowWinAPI(const HBITMAP& bitmap)
	{
		DrawImageWinAPI(bitmap, 0, 0, GetClientWidth(), GetClientHeight());
	}

	HBITMAP LoadHBitmap(const LPCWSTR filename)
	{
		HBITMAP hbm;
		Gdiplus::Bitmap bm(filename);
		bm.GetHBITMAP(Gdiplus::Color(0, 0, 0), &hbm);

		return hbm;
	}

protected:
	virtual VOID OnCreate() {};
	virtual VOID OnUpdate(float fdeltaTime) {};
	virtual VOID OnDestroy() {}

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

			for (UIElement* el : f_vuiElements)
			{
				el->Draw(this);
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

	std::vector<UIElement*> f_vuiElements;

private:
	bool f_bShowFps;

};

class Button : public UIElement
{
	void Draw(void* eng) const override
	{
		((Engine*)eng)->FillRectangleWinAPI(GetX(), GetY(), GetX() + GetWidth(), GetY() + GetHeight(), ((Engine*)eng)->ToCOLORREF(GetBackgroundColor()));
		((Engine*)eng)->DrawStringWinAPI(GetText().c_str(), GetX(), GetY(), GetX() + GetWidth(), GetY() + GetHeight(), ((Engine*)eng)->ToCOLORREF(GetForegroundColor()));
	}
};
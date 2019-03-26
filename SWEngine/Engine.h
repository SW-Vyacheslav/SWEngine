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

#include "GUI.h"
#include "Color.h"
#include "Events.h"
#include "Mathematics.h"

#pragma comment (lib,"Gdiplus.lib")

namespace SWEngine
{
	enum KeyCode
	{
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,
		MOUSELEFT = 0x01,
		MOUSERIGHT = 0x02,
		MOUSEMIDDLE = 0x04,
		BACKSPACE = 0x08,
		TAB = 0x09,
		ENTER = 0x0D,
		SHIFT = 0x10,
		CTRL = 0x11,
		ALT = 0x12,
		ESC = 0x1B,
		SPACE = 0x20,
		LEFTARROW = 0x25,
		UPARROW = 0x26,
		RIGHTARROW = 0x27,
		DOWNARROW = 0x28
	};
	enum PenStyle
	{
		SOLID = PS_SOLID,
		EMPTY = PS_NULL,
		DASH = PS_DASH,
		DOT = PS_DOT,
		DASHDOT = PS_DASHDOT,
		DASHDOTDOT = PS_DASHDOTDOT,
		INSIDEFRAME = PS_INSIDEFRAME
	};

	class KeyInfo
	{
	public:
		KeyInfo();

		bool KeyDown;
		bool KeyUp;
		bool KeyPressed;
	};

	class ScanLine
	{
	public:
		ScanLine();
		ScanLine(const int& x1, const int& y1, const int& x2, const int& y2);

		void SetY1(const int& y1);
		void SetY2(const int& y2);

		bool GetCrossPoint(const ScanLine& line, POINT& outpoint);

	private:
		int x1;
		int y1;
		int x2;
		int y2;
	};

	class Engine
	{
	public:
		Engine();

		VOID Start(UINT windowWidth, UINT windowHeight);
		UINT GetClientWidth() const;
		UINT GetClientHeight() const;
		INT GetMouseX() const;
		INT GetMouseY() const;
		VOID ShowFps(bool isShowed);
		bool IsFpsShowed() const;
		const KeyInfo* GetKeyInfo(const INT& keycode) const;

		//UI methods
		VOID AddUIElement(GUI::UIElement& el);

		//SWEngine drawing methods
		VOID DrawLine(const int& x1, const int& y1, const int& x2, const int& y2, const Color::IColorFormat& color);
		VOID DrawTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const Color::IColorFormat& color);
		VOID FillTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const Color::IColorFormat& color);
		VOID FillRectangle(const int& x1, const int& y1, const int& x2, const int& y2, const Color::IColorFormat& color);
		VOID DrawCircle(const int& cx, const int& cy, const int& radius, const Color::IColorFormat& color);

		//WinAPI drawing methods
		VOID FillCircleWinAPI(const int& cx, const int& cy, const int& radius, const COLORREF& color);
		VOID DrawPixelWinAPI(const int& x, const int& y, const COLORREF& color);
		VOID DrawLineWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const COLORREF& color, const UINT& width = 1, const INT& pen_style = PenStyle::SOLID);
		VOID DrawTriangleWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const COLORREF& color, const UINT& border_width = 1, const INT& pen_style = PenStyle::SOLID);
		VOID DrawStringWinAPI(LPCWSTR str, const int& x1, const int& y1, const int& x2, const int& y2, const COLORREF& textcolor);
		VOID DrawImageWinAPI(const HBITMAP& bitmap, const int& x1, const int& y1, const UINT& width, const UINT& height);
		VOID FillTriangleWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const COLORREF& color);
		VOID FillRectangleWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const COLORREF& color);
		VOID FillWindowWinAPI(const COLORREF& color);
		VOID FillWindowWinAPI(const HBITMAP& bitmap);
		HBITMAP LoadHBitmap(const LPCWSTR filename);

	protected:
		virtual VOID OnCreate();
		virtual VOID OnUpdate(float fdeltaTime);
		virtual VOID OnDestroy();

	private:
		static LRESULT CALLBACK StaticMainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		VOID EngineLoop();
		VOID SetCurrentBrushColor(const COLORREF& color);
		VOID SetCurrentPenColor(const COLORREF& color);
		VOID SetCurrentPenWidth(const INT& width);
		VOID SetCurrentPenStyle(const INT& style);

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

		std::vector<GUI::UIElement*> f_vuiElements;

	private:
		bool f_bShowFps;
	};
}
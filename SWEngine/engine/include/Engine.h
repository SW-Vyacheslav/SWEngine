#ifndef SWE_ENGINE
#define SWE_ENGINE

#include "Export.h"

#include <Windows.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <vector>

#include <gdiplus.h>
#include <objidl.h>
#pragma comment (lib,"Gdiplus.lib")

#include "drawing/Color.h"
#include "system/KeyInfo.h"

namespace swe
{
	class SWE_API Engine
	{
	public:
		Engine();
		~Engine();

		VOID Start(UINT windowWidth, UINT windowHeight);
		UINT GetClientWidth() const;
		UINT GetClientHeight() const;
		INT GetMouseX() const;
		INT GetMouseY() const;
		VOID ShowFps(bool isShowed);
		bool IsFpsShowed() const;
		system::KeyInfo GetKeyInfo(const INT& keycode) const;

		//SWEngine drawing methods
		VOID DrawLine(const int& x1, const int& y1, const int& x2, const int& y2, const drawing::Color& color);
		VOID DrawTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const drawing::Color& color);
		VOID DrawCircle(const int& cx, const int& cy, const int& radius, const drawing::Color& color);
		VOID DrawRectangle(const int& x1, const int& y1, const int& x2, const int& y2, const drawing::Color& color);
		VOID FillTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const drawing::Color& color);
		VOID FillRectangle(const int& x1, const int& y1, const int& x2, const int& y2, const drawing::Color& color);

		//WinAPI drawing methods
		VOID DrawPixelWinAPI(const int& x, const int& y, const drawing::Color& color);
		VOID DrawLineWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const drawing::Color& color, const UINT& width = 1);
		VOID DrawTriangleWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const drawing::Color& color, const UINT& border_width = 1);
		VOID DrawStringWinAPI(LPCWSTR str, const int& x1, const int& y1, const int& x2, const int& y2, const drawing::Color& textcolor);
		VOID DrawImageWinAPI(const LPCWSTR filename, const int& x, const int& y, const UINT& width, const UINT& height);
		VOID FillCircleWinAPI(const int& cx, const int& cy, const int& radius, const drawing::Color& color);
		VOID FillTriangleWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, const drawing::Color& color);
		VOID FillRectangleWinAPI(const int& x1, const int& y1, const int& x2, const int& y2, const drawing::Color& color);
		VOID FillWindowWinAPI(const drawing::Color& color);

	protected:
		virtual VOID OnCreate();
		virtual VOID OnUpdate(float fdeltaTime);
		virtual VOID OnDestroy();

	private:
		static LRESULT CALLBACK StaticMainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		VOID EngineLoop();

	private:
		HINSTANCE f_hInstance;
		HWND f_hMainWnd;
		HDC f_hMainWndDC;
		HDC f_hBufferDC;
		HBITMAP f_hBufferBitmap;

		Gdiplus::Graphics* f_gdipGraphics;

		UINT f_uiClientWidth;
		UINT f_uiClientHeight;
		std::chrono::system_clock::time_point f_timePoint1;
		std::chrono::system_clock::time_point f_timePoint2;

		system::KeyInfo f_keys[256];
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
}

#endif
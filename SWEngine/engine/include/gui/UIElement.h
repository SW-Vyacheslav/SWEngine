#pragma once
#include "../Event.h"
#include "../drawing/Color.h"
#include <string.h>

namespace SWEngine
{
	namespace GUI
	{
		class UIElement abstract
		{
		public:
			UIElement();
			UIElement(const int& x, const int& y, const int& width, const int& height);
			~UIElement();

			void SetX(const int& x);
			void SetY(const int& y);
			void SetWidth(const int& width);
			void SetHeigth(const int& height);
			void SetText(const std::wstring& text);
			void SetMouseHover(const bool& isHover);

			int GetX() const;
			int GetY() const;
			int GetWidth() const;
			int GetHeight() const;
			const Drawing::Color GetBackgroundColor() const;
			const Drawing::Color GetForegroundColor() const;
			const Drawing::Color GetMouseHoverColor() const;
			std::wstring GetText() const;

			virtual void UpdateElement(void* engine) = 0;

			bool IsMouseHover() const;

		public:
			Event onMouseDown;
			Event onMouseUp;
			Event onMousePressed;
			Event onMouseHover;

		protected:
			int x;
			int y;
			int width;
			int height;
			bool isMouseHover;
			std::wstring text;
			Drawing::Color backgroundColor;
			Drawing::Color foregroundColor;
			Drawing::Color mouseHoverColor;
		};
	}
}
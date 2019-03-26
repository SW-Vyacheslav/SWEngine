#pragma once
#include "Events.h"
#include "Color.h"

namespace SWEngine
{
	namespace GUI
	{
		class UIElement abstract
		{
		public:
			UIElement();
			UIElement(const int& x, const int& y, const int& width, const int& height);

			void SetX(const int& x);
			void SetY(const int& y);
			void SetWidth(const int& width);
			void SetHeigth(const int& height);
			void SetText(const std::wstring& text);
			void SetMouseHover(const bool& isHover);

			const int& GetX() const;
			const int& GetY() const;
			const int& GetWidth() const;
			const int& GetHeight() const;
			const Color::RGBColorFormat& GetBackgroundColor() const;
			const Color::RGBColorFormat& GetForegroundColor() const;
			const std::wstring& GetText() const;

			virtual void UpdateElement(void* engine) = 0;

			bool IsMouseHover() const;

		public:
			Events::Event onMouseDown;
			Events::Event onMouseUp;
			Events::Event onMousePressed;
			Events::Event onMouseHover;

		protected:
			int x;
			int y;
			int width;
			int height;
			bool isMouseHover;
			std::wstring text;
			Color::RGBColorFormat backgroundColor;
			Color::RGBColorFormat foregroundColor;
		};

		class Button : public UIElement
		{
		public:
			Button(const int& x, const int& y, const int& width, const int& height);

			void UpdateElement(void* engine) override;
		};
	}
}
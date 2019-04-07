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
			const Color::IColorFormat* GetBackgroundColor() const;
			const Color::IColorFormat* GetForegroundColor() const;
			const Color::IColorFormat* GetMouseHoverColor() const;
			std::wstring GetText() const;

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
			Color::IColorFormat* backgroundColor;
			Color::IColorFormat* foregroundColor;
			Color::IColorFormat* mouseHoverColor;
		};

		class Button : public UIElement
		{
		public:
			Button(const int& x, const int& y, const int& width, const int& height);

			void UpdateElement(void* engine) override;
		};
	}
}
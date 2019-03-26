#include "GUI.h"
#include "Engine.h"

namespace SWEngine
{
	namespace GUI
	{
		UIElement::UIElement() : x(0), y(0), width(10), height(10), isMouseHover(false), backgroundColor(Color::Colors::GREY), foregroundColor(Color::Colors::BLACK) {}
		UIElement::UIElement(const int& x, const int& y, const int& width, const int& height) : isMouseHover(false), backgroundColor(Color::Colors::GREY), foregroundColor(Color::Colors::BLACK)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}
		void UIElement::SetX(const int& x)
		{
			this->x = x;
		}
		void UIElement::SetY(const int& y)
		{
			this->y = y;
		}
		void UIElement::SetWidth(const int& width)
		{
			this->width = width;
		}
		void UIElement::SetHeigth(const int& height)
		{
			this->height = height;
		}
		void UIElement::SetText(const std::wstring& text)
		{
			this->text = text;
		}
		void UIElement::SetMouseHover(const bool& isHover)
		{
			isMouseHover = isHover;
		}
		const int& UIElement::GetX() const
		{
			return x;
		}
		const int& UIElement::GetY() const
		{
			return y;
		}
		const int& UIElement::GetWidth() const
		{
			return width;
		}
		const int& UIElement::GetHeight() const
		{
			return height;
		}
		const Color::RGBColorFormat& UIElement::GetBackgroundColor() const
		{
			return backgroundColor;
		}
		const Color::RGBColorFormat& UIElement::GetForegroundColor() const
		{
			return foregroundColor;
		}
		const std::wstring& UIElement::GetText() const
		{
			return text;
		}
		bool UIElement::IsMouseHover() const
		{
			return isMouseHover;
		}
		
		Button::Button(const int& x, const int& y, const int& width, const int& height) : UIElement(x, y, width, height) {}
		void Button::UpdateElement(void* engine)
		{
			if (IsMouseHover())
			{
				((Engine*)engine)->FillRectangleWinAPI(x, y, x + width, y + height, Color::Colors::AQUA.ToCOLORREF());
			}
			else ((Engine*)engine)->FillRectangleWinAPI(x, y, x + width, y + height, backgroundColor.ToCOLORREF());
			((Engine*)engine)->DrawStringWinAPI(text.c_str(), x, y, x + width, y + height, foregroundColor.ToCOLORREF());
		}
	}
}
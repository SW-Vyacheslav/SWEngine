#include "../../include/gui/UIElement.h"
#include "../../include/drawing/Color.h"

namespace SWEngine
{
	namespace GUI
	{
		UIElement::UIElement() : x(0), y(0), width(10), height(10), isMouseHover(false)
		{
			backgroundColor = Drawing::Color::FromHEX("#cfd8dc");
			foregroundColor = Drawing::Colors::BLACK;
			mouseHoverColor = Drawing::Color::FromHEX("#9ea7aa");
		}
		UIElement::UIElement(const int& x, const int& y, const int& width, const int& height) : isMouseHover(false)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
			backgroundColor = Drawing::Color::FromHEX("#cfd8dc");
			foregroundColor = Drawing::Colors::BLACK;
			mouseHoverColor = Drawing::Color::FromHEX("#9ea7aa");
		}
		UIElement::~UIElement() {}
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
		int UIElement::GetX() const
		{
			return x;
		}
		int UIElement::GetY() const
		{
			return y;
		}
		int UIElement::GetWidth() const
		{
			return width;
		}
		int UIElement::GetHeight() const
		{
			return height;
		}
		const Drawing::Color UIElement::GetBackgroundColor() const
		{
			return backgroundColor;
		}
		const Drawing::Color UIElement::GetForegroundColor() const
		{
			return foregroundColor;
		}
		const Drawing::Color UIElement::GetMouseHoverColor() const
		{
			return mouseHoverColor;
		}
		std::wstring UIElement::GetText() const
		{
			return text;
		}
		bool UIElement::IsMouseHover() const
		{
			return isMouseHover;
		}
	}
}
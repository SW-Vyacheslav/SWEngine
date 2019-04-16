#pragma once
#include <string>

namespace SWEngine
{
	namespace Drawing
	{
		class Color
		{
		public:
			Color();
			Color(const Color& color);
			~Color();

			static Color FromRGBA(const int& red, const int& green, const int& blue, const int& alpha = 255);
			static Color FromHSL(const int& hue, const int& saturation, const int& lightness);
			static Color FromHEX(const std::string& hex);

			int GetAlpha() const;
			int GetRed() const;
			int GetGreen() const;
			int GetBlue() const;

			int GetHue() const;
			int GetSaturation() const;
			int GetLightness() const;

			unsigned long GetValue() const;

		private:
			unsigned long val;
			static const unsigned char alpha_shift;
			static const unsigned char red_shift;
			static const unsigned char green_shift;
		};

		class Colors
		{
		public:
			static const Color RED;
			static const Color GREEN;
			static const Color BLUE;
			static const Color BLACK;
			static const Color WHITE;
			static const Color YELLOW;
			static const Color MAGENTA;
			static const Color AQUA;
			static const Color GREY;
		};
	}
}
#include "../../include/drawing/Color.h"

namespace swe
{
	namespace drawing
	{
		Color::Color() : val(255 << alpha_shift) {}
		Color::Color(const Color& color)
		{
			val = color.val;
		}
		Color::~Color() {}
		Color Color::FromRGB(const int& red, const int& green, const int& blue, const int& alpha)
		{
			Color ret_val;
			if ((red >= 0 && red <= 255) && (green >= 0 && green <= 255) && (blue >= 0 && blue <= 255) && (alpha >= 0 && alpha <= 255))
			{
				ret_val.val |= alpha << alpha_shift;
				ret_val.val |= red << red_shift;
				ret_val.val |= green << green_shift;
				ret_val.val |= blue;
			}
			return ret_val;
		}
		Color Color::FromHSL(const int& hue, const int& saturation, const int& lightness, const int& alpha)
		{
			Color ret_val;
			return ret_val;
		}
		Color Color::FromHEX(const std::string& hex)
		{
			Color ret_val;

			if (hex[0] == '#')
			{
				for (int i = 1; i < (int)hex.size(); i++)
				{
					if ((hex[i] >= '0' && hex[i] <= '9') || (hex[i] >= 'a' && hex[i] <= 'f')) continue;
					else return ret_val;
				}

				int num = 0;

				if (hex.size() == 7)
				{
					ret_val.val |= 255 << alpha_shift;

					if (hex[1] >= 'a' && hex[1] <= 'f') num = (hex[1] - 87) << 4;
					else num = atoi(&hex[1]) << 4;
					if (hex[2] >= 'a' && hex[2] <= 'f') num |= hex[2] - 87;
					else num |= atoi(&hex[2]);
					ret_val.val |= num << red_shift;

					if (hex[3] >= 'a' && hex[3] <= 'f') num = (hex[3] - 87) << 4;
					else num = atoi(&hex[3]) << 4;
					if (hex[4] >= 'a' && hex[4] <= 'f') num |= hex[4] - 87;
					else num |= atoi(&hex[4]);
					ret_val.val |= num << green_shift;

					if (hex[5] >= 'a' && hex[5] <= 'f') num = (hex[5] - 87) << 4;
					else num = atoi(&hex[5]) << 4;
					if (hex[6] >= 'a' && hex[6] <= 'f') num |= hex[6] - 87;
					else num |= atoi(&hex[6]);
					ret_val.val |= num;
				}
				else if (hex.size() == 9)
				{
					if (hex[1] >= 'a' && hex[1] <= 'f') num = (hex[1] - 87) << 4;
					else num = atoi(&hex[1]) << 4;
					if (hex[2] >= 'a' && hex[2] <= 'f') num |= hex[2] - 87;
					else num |= atoi(&hex[2]);
					ret_val.val |= num << alpha_shift;

					if (hex[3] >= 'a' && hex[3] <= 'f') num = (hex[3] - 87) << 4;
					else num = atoi(&hex[3]) << 4;
					if (hex[4] >= 'a' && hex[4] <= 'f') num |= hex[4] - 87;
					else num |= atoi(&hex[4]);
					ret_val.val |= num << red_shift;

					if (hex[5] >= 'a' && hex[5] <= 'f') num = (hex[5] - 87) << 4;
					else num = atoi(&hex[5]) << 4;
					if (hex[6] >= 'a' && hex[6] <= 'f') num |= hex[6] - 87;
					else num |= atoi(&hex[6]);
					ret_val.val |= num << green_shift;

					if (hex[7] >= 'a' && hex[7] <= 'f') num = (hex[7] - 87) << 4;
					else num = atoi(&hex[7]) << 4;
					if (hex[8] >= 'a' && hex[8] <= 'f') num |= hex[8] - 87;
					else num |= atoi(&hex[8]);
					ret_val.val |= num;
				}
			}

			return ret_val;
		}
		int Color::GetAlpha() const
		{
			return (int)((val & 0xff000000) >> alpha_shift);
		}
		int Color::GetRed() const
		{
			return (int)((val & 0xff0000) >> red_shift);
		}
		int Color::GetGreen() const
		{
			return (int)((val & 0xff00) >> green_shift);
		}
		int Color::GetBlue() const
		{
			return (int)(val & 0xff);
		}
		int Color::GetHue() const
		{
			return 0;
		}
		int Color::GetSaturation() const
		{
			return 0;
		}
		int Color::GetLightness() const
		{
			return 0;
		}
		UInt32 Color::GetValue() const
		{
			return val;
		}

		const UInt8 Color::alpha_shift = 24;
		const UInt8 Color::red_shift = 16;
		const UInt8 Color::green_shift = 8;

		const Color Colors::RED = Color::FromRGB(255, 0, 0);
		const Color Colors::GREEN = Color::FromRGB(0, 255, 0);
		const Color Colors::BLUE = Color::FromRGB(0, 0, 255);
		const Color Colors::BLACK = Color::FromRGB(0, 0, 0);
		const Color Colors::WHITE = Color::FromRGB(255, 255, 255);
		const Color Colors::YELLOW = Color::FromRGB(255, 255, 0);
		const Color Colors::MAGENTA = Color::FromRGB(255, 0, 255);
		const Color Colors::AQUA = Color::FromRGB(0, 255, 255);
		const Color Colors::GREY = Color::FromRGB(128, 128, 128);
	}
}
#include "Color.h"

namespace SWEngine
{
	namespace Color
	{
		RGBValue::RGBValue() : r(0), g(0), b(0) {}
		RGBValue::RGBValue(const int& r, const int& g, const int& b)
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
		RGBValue::RGBValue(const RGBValue& val)
		{
			this->r = val.r;
			this->g = val.g;
			this->b = val.b;
		}
		const int& RGBValue::GetR() const
		{
			return r;
		}
		const int& RGBValue::GetG() const
		{
			return g;
		}
		const int& RGBValue::GetB() const
		{
			return b;
		}
		void RGBValue::SetR(const int& r)
		{
			if (r >= 0 && r <= 255) this->r = r;
		}
		void RGBValue::SetG(const int& g)
		{
			if (g >= 0 && g <= 255) this->g = g;
		}
		void RGBValue::SetB(const int& b)
		{
			if (b >= 0 && b <= 255) this->b = b;
		}
		void RGBValue::SetRGB(const int& r, const int& g, const int& b)
		{
			if ((r >= 0 && r <= 255) && (g >= 0 && g <= 255) && (b >= 0 && b <= 255))
			{
				this->r = r;
				this->g = g;
				this->b = b;
			}
		}
		void RGBValue::SetRGB(const RGBValue& val)
		{
			this->r = val.r;
			this->g = val.g;
			this->b = val.b;
		}

		HSLValue::HSLValue() : h(0), s(0), l(0) {}
		HSLValue::HSLValue(const int& h, const int& s, const int& l)
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
		HSLValue::HSLValue(const HSLValue& val)
		{
			this->h = val.h;
			this->s = val.s;
			this->l = val.l;
		}
		const int& HSLValue::GetH() const
		{
			return h;
		}
		const int& HSLValue::GetS() const
		{
			return s;
		}
		const int& HSLValue::GetL() const
		{
			return l;
		}
		void HSLValue::SetH(const int& h)
		{
			if (h >= 0 && h <= 360) this->h = h;
		}
		void HSLValue::SetS(const int& s)
		{
			if (s >= 0 && s <= 100) this->s = s;
		}
		void HSLValue::SetL(const int& l)
		{
			if (l >= 0 && l <= 100) this->l = l;
		}
		void HSLValue::SetHSL(const int& h, const int& s, const int& l)
		{
			if ((h >= 0 && h <= 360) && (s >= 0 && s <= 100) && (l >= 0 && l <= 100))
			{
				this->h = h;
				this->s = s;
				this->l = l;
			}
		}
		void HSLValue::SetHSL(const HSLValue& val)
		{
			this->h = val.h;
			this->s = val.s;
			this->l = val.l;
		}

		RGBColorFormat::RGBColorFormat() : RGBValue() {}
		RGBColorFormat::RGBColorFormat(const int& r, const int& g, const int& b) : RGBValue(r, g, b) {}
		RGBColorFormat::RGBColorFormat(const RGBValue& val) : RGBValue(val.GetR(), val.GetG(), val.GetB()) {}
		void RGBColorFormat::ToRGBColorFormat(RGBValue& val) const
		{
			val.SetR(GetR());
			val.SetG(GetG());
			val.SetB(GetB());
		}
		void RGBColorFormat::ToHSLColorFormat(HSLValue& val) const
		{

		}
		void RGBColorFormat::ToHEXColorFormat(std::string& hex) const
		{

		}
		COLORREF RGBColorFormat::ToCOLORREF() const
		{
			return RGB(GetR(), GetG(), GetB());
		}

		HSLColorFormat::HSLColorFormat() : HSLValue() {}
		HSLColorFormat::HSLColorFormat(const int& h, const int& s, const int& l) : HSLValue(h, s, l) {}
		HSLColorFormat::HSLColorFormat(const HSLValue& val) : HSLValue(val.GetH(), val.GetS(), val.GetL()) {}
		void HSLColorFormat::ToRGBColorFormat(RGBValue& val) const
		{
			int r = 0;
			int g = 0;
			int b = 0;

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

			val.SetR(r);
			val.SetG(g);
			val.SetB(b);
		}
		void HSLColorFormat::ToHSLColorFormat(HSLValue& val) const
		{
			val.SetH(GetH());
			val.SetS(GetS());
			val.SetL(GetL());
		}
		void HSLColorFormat::ToHEXColorFormat(std::string& hex) const
		{

		}
		COLORREF HSLColorFormat::ToCOLORREF() const
		{
			RGBValue val;
			ToRGBColorFormat(val);
			return RGB(val.GetR(), val.GetG(), val.GetB());
		}

		HEXColorFormat::HEXColorFormat() : hex("#000000") {}
		HEXColorFormat::HEXColorFormat(const std::string& hex)
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
		void HEXColorFormat::ToRGBColorFormat(RGBValue& val) const
		{
			int r = 0;
			int g = 0;
			int b = 0;

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

			val.SetR(r);
			val.SetG(g);
			val.SetB(b);
		}
		void HEXColorFormat::ToHSLColorFormat(HSLValue& val) const
		{

		}
		void HEXColorFormat::ToHEXColorFormat(std::string& hex) const
		{
			hex = this->hex;
		}
		COLORREF HEXColorFormat::ToCOLORREF() const
		{
			RGBValue val;
			ToRGBColorFormat(val);
			return RGB(val.GetR(), val.GetG(), val.GetB());
		}

		
		const RGBColorFormat Colors::RED = { 255,0,0 };
		const RGBColorFormat Colors::GREEN = { 0, 255, 0 };
		const RGBColorFormat Colors::BLUE = { 0, 0, 255 };
		const RGBColorFormat Colors::BLACK = { 0, 0, 0 };
		const RGBColorFormat Colors::WHITE = { 255, 255, 255 };
		const RGBColorFormat Colors::YELLOW = { 255, 255, 0 };
		const RGBColorFormat Colors::MAGENTA = { 255, 0, 255 };
		const RGBColorFormat Colors::AQUA = { 0, 255, 255 };
		const RGBColorFormat Colors::GREY = { 128, 128, 128 };
	}
}
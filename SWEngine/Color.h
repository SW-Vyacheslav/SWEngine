#pragma once
#include <Windows.h>
#include <string>

#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

namespace SWEngine
{
	namespace Color
	{
		class RGBValue
		{
		public:
			RGBValue();
			RGBValue(const int& r, const int& g, const int& b);
			RGBValue(const RGBValue& val);
			~RGBValue();

			int GetR() const;
			int GetG() const;
			int GetB() const;
			void SetR(const int& r);
			void SetG(const int& g);
			void SetB(const int& b);
			void SetRGB(const int& r, const int& g, const int& b);
			void SetRGB(const RGBValue& val);

		private:
			int r;
			int g;
			int b;
		};
		class HSLValue
		{
		public:
			HSLValue();
			HSLValue(const int& h, const int& s, const int& l);
			HSLValue(const HSLValue& val);
			~HSLValue();

			int GetH() const;
			int GetS() const;
			int GetL() const;
			void SetH(const int& h);
			void SetS(const int& s);
			void SetL(const int& l);
			void SetHSL(const int& h, const int& s, const int& l);
			void SetHSL(const HSLValue& val);

		private:
			int h;
			int s;
			int l;
		};

		class IColorFormat abstract
		{
		public:
			virtual void ToRGBColorFormat(RGBValue& val) const = 0;
			virtual void ToHSLColorFormat(HSLValue& val) const = 0;
			virtual void ToHEXColorFormat(std::string& hex) const = 0;
			virtual COLORREF ToCOLORREF() const = 0;
		};
		class RGBColorFormat : public IColorFormat, public RGBValue
		{
		public:
			RGBColorFormat();
			RGBColorFormat(const int& r, const int& g, const int& b);
			RGBColorFormat(const RGBValue& val);
			RGBColorFormat(const RGBColorFormat& format);
			~RGBColorFormat();

			void ToRGBColorFormat(RGBValue& val) const override;
			void ToHSLColorFormat(HSLValue& val) const override;
			void ToHEXColorFormat(std::string& hex) const override;
			COLORREF ToCOLORREF() const override;
		};
		class HSLColorFormat : public IColorFormat, public HSLValue
		{
		public:
			HSLColorFormat();
			HSLColorFormat(const int& h, const int& s, const int& l);
			HSLColorFormat(const HSLValue& val);
			HSLColorFormat(const HSLColorFormat& format);
			~HSLColorFormat();

			void ToRGBColorFormat(RGBValue& val) const override;
			void ToHSLColorFormat(HSLValue& val) const override;
			void ToHEXColorFormat(std::string& hex) const override;
			COLORREF ToCOLORREF() const override;
		};
		class HEXColorFormat : public IColorFormat
		{
		public:
			HEXColorFormat();
			HEXColorFormat(const std::string& hex);
			HEXColorFormat(const HEXColorFormat& format);
			~HEXColorFormat();

			void ToRGBColorFormat(RGBValue& val) const override;
			void ToHSLColorFormat(HSLValue& val) const override;
			void ToHEXColorFormat(std::string& hex) const override;
			COLORREF ToCOLORREF() const override;

		private:
			std::string hex;
		};

		class Colors
		{
		public:
			static const RGBColorFormat RED;
			static const RGBColorFormat GREEN;
			static const RGBColorFormat BLUE;
			static const RGBColorFormat BLACK;
			static const RGBColorFormat WHITE;
			static const RGBColorFormat YELLOW;
			static const RGBColorFormat MAGENTA;
			static const RGBColorFormat AQUA;
			static const RGBColorFormat GREY;
		};

		class Color
		{
		public:
			Color();
			Color(const Color& color);
			~Color();

			static Color FromRGB(const int& red, const int& green, const int& blue);
			static Color FromHSL(const int& hue, const int& saturation, const int& lightness);
			static Color FromHEX(const std::string& hex);

			int GetAlpha();
			int GetRed();
			int GetGreen();
			int GetBlue();

			int GetHue();
			int GetSaturation();
			int GetLightness();

		private:
			unsigned long value;
		};
	}
}
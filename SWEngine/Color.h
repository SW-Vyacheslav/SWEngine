#pragma once
#include <Windows.h>
#include <string>

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

			const int& GetR() const;
			const int& GetG() const;
			const int& GetB() const;
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

			const int& GetH() const;
			const int& GetS() const;
			const int& GetL() const;
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
	}
}
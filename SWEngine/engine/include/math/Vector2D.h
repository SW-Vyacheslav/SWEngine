#pragma once
#include "Matrix3x3.h"

namespace SWEngine
{
	namespace Math
	{
		class Vector2D
		{
		public:
			Vector2D();
			Vector2D(const float& x, const float& y);
			Vector2D(const Vector2D& vec);
			~Vector2D();

			void SetX(const float& x);
			void SetY(const float& y);
			void SetW(const float& w);
			float GetX() const;
			float GetY() const;
			float GetW() const;

			float GetLength() const;
			Vector2D Normalize() const;

			Vector2D operator+(const Vector2D& vec);
			Vector2D operator-(const Vector2D& vec);
			Vector2D operator*(const float& num);
			Vector2D operator*(const Matrix3x3& mat);

		private:
			float x;
			float y;
			float w;
		};
	}
}
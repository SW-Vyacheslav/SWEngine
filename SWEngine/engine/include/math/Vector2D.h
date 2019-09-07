#ifndef SWE_VECTOR2D
#define SWE_VECTOR2D

#include "Matrix3x3.h"
#include "../Export.h"

namespace swe
{
	namespace Math
	{
		class SWE_API Vector2D
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

			static float DotProduct(const Vector2D& vec1, const Vector2D& vec2);

			Vector2D operator+(const Vector2D& vec);
			void operator+=(const Vector2D& vec);
			Vector2D operator-(const Vector2D& vec);
			void operator-=(const Vector2D& vec);
			Vector2D operator*(const float& num);
			void operator*=(const float& num);
			Vector2D operator*(const Matrix3x3& mat);
			void operator*=(const Matrix3x3& mat);

		private:
			float x;
			float y;
			float w;
		};
	}
}

#endif
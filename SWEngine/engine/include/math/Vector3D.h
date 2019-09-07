#ifndef SWE_VECTOR3D
#define SWE_VECTOR3D

#include "Matrix4x4.h"
#include "../Export.h"

namespace swe
{
	namespace Math
	{
		class SWE_API Vector3D
		{
		public:
			Vector3D();
			Vector3D(const float& x, const float& y, const float& z);
			Vector3D(const Vector3D& vec);
			~Vector3D();

			void SetX(const float& x);
			void SetY(const float& y);
			void SetZ(const float& z);
			void SetW(const float& w);
			float GetX() const;
			float GetY() const;
			float GetZ() const;
			float GetW() const;

			float GetLength() const;
			Vector3D Normalize() const;

			static float DotProduct(const Vector3D& vec1, const Vector3D& vec2);
			static Vector3D CrossProduct(const Vector3D& vec1, const Vector3D& vec2);

			Vector3D operator+(const Vector3D& vec);
			void operator+=(const Vector3D& vec);
			Vector3D operator-(const Vector3D& vec);
			void operator-=(const Vector3D& vec);
			Vector3D operator*(const float& num);
			void operator*=(const float& num);
			Vector3D operator*(const Matrix4x4& mat);
			void operator*=(const Matrix4x4& mat);

		private:
			float x;
			float y;
			float z;
			float w;
		};
	}
}

#endif

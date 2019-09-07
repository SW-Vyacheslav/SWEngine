#ifndef SWE_MATRIX4X4
#define SWE_MATRIX4X4

#include "../Export.h"

namespace swe
{
	namespace Math
	{
		class SWE_API Matrix4x4
		{
		public:
			Matrix4x4();
			Matrix4x4(const Matrix4x4& mat);
			~Matrix4x4();

			void SetElementAt(const int& row, const int& column, const float& val);
			float GetElementAt(const int& row, const int& column) const;

			Matrix4x4 operator*(const Matrix4x4& mat);
			void operator*=(const Matrix4x4& mat);

		private:
			float grid[4][4];
		};
	}
}

#endif
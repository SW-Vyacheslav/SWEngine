#pragma once

namespace SWEngine
{
	namespace Math
	{
		class Matrix3x3
		{
		public:
			Matrix3x3();
			Matrix3x3(const Matrix3x3& mat);
			~Matrix3x3();

			void SetElementAt(const int& row, const int& column, const float& val);
			float GetElementAt(const int& row, const int& column) const;

			Matrix3x3 operator*(const Matrix3x3& mat);

		private:
			float grid[3][3];
		};
	}
}
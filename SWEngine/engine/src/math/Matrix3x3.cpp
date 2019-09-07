#include "../../include/math/Matrix3x3.h"

namespace swe
{
	namespace Math
	{
		Matrix3x3::Matrix3x3() : grid{ 0.0f } {}
		Matrix3x3::Matrix3x3(const Matrix3x3& mat)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					grid[i][j] = mat.grid[i][j];
				}
			}
		}
		Matrix3x3::~Matrix3x3() {}

		void Matrix3x3::SetElementAt(const int& row, const int& column, const float& val)
		{
			if (row < 0 || row > 2) throw "Out of matrix row range";
			if (column < 0 || column > 2) throw "Out of matrix column range";

			grid[row][column] = val;
		}
		float Matrix3x3::GetElementAt(const int& row, const int& column) const
		{
			if (row < 0 || row > 2) throw "Out of matrix row range";
			if (column < 0 || column > 2) throw "Out of matrix column range";

			return grid[row][column];
		}

		Matrix3x3 Matrix3x3::operator*(const Matrix3x3& mat)
		{
			Matrix3x3 ret_val;
			float sum = 0.0f;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						sum += GetElementAt(i, k) * mat.GetElementAt(k, j);
					}
					ret_val.SetElementAt(i, j, sum);
					sum = 0.0f;
				}
			}
			return ret_val;
		}
		void Matrix3x3::operator*=(const Matrix3x3& mat)
		{
			Matrix3x3 temp = *this;
			float sum = 0.0f;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						sum += temp.GetElementAt(i, k) * mat.GetElementAt(k, j);
					}
					SetElementAt(i, j, sum);
					sum = 0.0f;
				}
			}
		}
	}
}
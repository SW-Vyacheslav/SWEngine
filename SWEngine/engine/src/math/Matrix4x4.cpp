#include "../../include/math/Matrix4x4.h"

namespace swe
{
	namespace Math
	{
		Matrix4x4::Matrix4x4() : grid{0.0f} {}
		Matrix4x4::Matrix4x4(const Matrix4x4& mat) 
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					grid[i][j] = mat.grid[i][j];
				}
			}
		}
		Matrix4x4::~Matrix4x4() {}

		void Matrix4x4::SetElementAt(const int& row, const int& column, const float& val)
		{
			if (row < 0 || row > 3) throw "Out of matrix row range";
			if (column < 0 || column > 3) throw "Out of matrix column range";

			grid[row][column] = val;
		}
		float Matrix4x4::GetElementAt(const int& row, const int& column) const
		{
			if (row < 0 || row > 3) throw "Out of matrix row range";
			if (column < 0 || column > 3) throw "Out of matrix column range";

			return grid[row][column];
		}

		Matrix4x4 Matrix4x4::operator*(const Matrix4x4& mat)
		{
			Matrix4x4 ret_val;
			float sum = 0.0f;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					for (int k = 0; k < 4; k++)
					{
						sum += GetElementAt(i, k) * mat.GetElementAt(k, j);
					}
					ret_val.SetElementAt(i, j, sum);
					sum = 0.0f;
				}
			}
			return ret_val;
		}
		void Matrix4x4::operator*=(const Matrix4x4& mat)
		{
			Matrix4x4 temp = *this;
			float sum = 0.0f;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					for (int k = 0; k < 4; k++)
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
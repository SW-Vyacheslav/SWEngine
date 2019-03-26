#include "Mathematics.h"
#include <math.h>

namespace SWEngine
{
	namespace Mathematics
	{
		Vec2D::Vec2D() : x(0.0f), y(0.0f), w(1.0f) {}
		Vec2D::Vec2D(const float& x, const float& y) : x(x), y(y), w(1.0f) {}
		Vec2D::Vec2D(const Vec2D& vec)
		{
			x = vec.GetX();
			y = vec.GetY();
			w = vec.GetW();
		}
		void Vec2D::SetX(const float& x)
		{
			this->x = x;
		}
		void Vec2D::SetY(const float& y)
		{
			this->y = y;
		}
		void Vec2D::SetW(const float& w)
		{
			this->w = w;
		}
		float Vec2D::GetX() const
		{
			return x;
		}
		float Vec2D::GetY() const
		{
			return y;
		}
		float Vec2D::GetW() const
		{
			return w;
		}

		Mat3x3::Mat3x3() : grid{ 0.0f } {}
		Mat3x3::Mat3x3(const Mat3x3& mat)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					grid[i][j] = mat.grid[i][j];
				}
			}
		}
		void Mat3x3::SetElementAt(const int& row, const int& column, const float& val)
		{
			if (row < 0 || row > 3) throw "Out of matrix row range";
			if (column < 0 || column > 3) throw "Out of matrix column range";

			grid[row][column] = val;
		}
		float Mat3x3::GetElementAt(const int& row, const int& column) const
		{
			if (row < 0 || row > 3) throw "Out of matrix row range";
			if (column < 0 || column > 3) throw "Out of matrix column range";

			return grid[row][column];
		}

		float Convert::DegreesToRadians(const float& deg)
		{
			return deg * (3.14159f / 180.0f);
		}

		void vecsum(Vec2D& out, const Vec2D& vec1, const Vec2D& vec2)
		{
			out.SetX(vec1.GetX() + vec2.GetX());
			out.SetY(vec1.GetY() + vec2.GetY());
		}
		void vecsum(Vec2D& vec1, const Vec2D& vec2)
		{
			vec1.SetX(vec1.GetX() + vec2.GetX());
			vec1.SetY(vec1.GetY() + vec2.GetY());
		}
		void vecsub(Vec2D& out, const Vec2D& vec1, const Vec2D& vec2)
		{
			out.SetX(vec1.GetX() - vec2.GetX());
			out.SetY(vec1.GetY() - vec2.GetY());
		}
		void vecsub(Vec2D& vec1, const Vec2D& vec2)
		{
			vec1.SetX(vec1.GetX() - vec2.GetX());
			vec1.SetY(vec1.GetY() - vec2.GetY());
		}
		void vecmul(Vec2D& out, const Vec2D& vec, const float& num)
		{
			out.SetX(vec.GetX() * num);
			out.SetY(vec.GetY() * num);
		}
		void vecmul(Vec2D& out, const Vec2D& vec, const Mat3x3& mat)
		{
			out.SetX(vec.GetX()*mat.GetElementAt(0, 0) + vec.GetY()*mat.GetElementAt(1, 0) + vec.GetW()*mat.GetElementAt(2, 0));
			out.SetY(vec.GetX()*mat.GetElementAt(0, 1) + vec.GetY()*mat.GetElementAt(1, 1) + vec.GetW()*mat.GetElementAt(2, 1));
			out.SetW(vec.GetX()*mat.GetElementAt(0, 2) + vec.GetY()*mat.GetElementAt(1, 2) + vec.GetW()*mat.GetElementAt(2, 2));
		}
		void vecmul(Vec2D& vec, const Mat3x3& mat)
		{
			Vec2D temp = vec;
			vec.SetX(temp.GetX()*mat.GetElementAt(0, 0) + temp.GetY()*mat.GetElementAt(1, 0) + temp.GetW()*mat.GetElementAt(2, 0));
			vec.SetY(temp.GetX()*mat.GetElementAt(0, 1) + temp.GetY()*mat.GetElementAt(1, 1) + temp.GetW()*mat.GetElementAt(2, 1));
			vec.SetW(temp.GetX()*mat.GetElementAt(0, 2) + temp.GetY()*mat.GetElementAt(1, 2) + temp.GetW()*mat.GetElementAt(2, 2));
		}
		void matmul(Mat3x3& out, const Mat3x3& mat1, const Mat3x3& mat2)
		{
			float sum = 0.0f;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						sum += mat1.GetElementAt(i, k) * mat2.GetElementAt(k, j);
					}
					out.SetElementAt(i, j, sum);
					sum = 0.0f;
				}
			}
		}
		void matmul(Mat3x3& mat1, const Mat3x3& mat2)
		{
			Mat3x3 temp = mat1;

			float sum = 0.0f;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						sum += temp.GetElementAt(i, k) * mat2.GetElementAt(k, j);
					}
					mat1.SetElementAt(i, j, sum);
					sum = 0.0f;
				}
			}
		}
		float veclen(const Vec2D& vec)
		{
			return sqrtf((vec.GetX()*vec.GetX()) + (vec.GetY()*vec.GetY()));
		}
		void vecnormalize(Vec2D& out)
		{
			float len = veclen(out);
			if (len == 0) return;
			out.SetX(out.GetX() / len);
			out.SetY(out.GetY() / len);
		}
		void setrotationmat(Mat3x3& out, const float& angle)
		{
			out.SetElementAt(0, 0, cosf(Convert::DegreesToRadians(angle)));
			out.SetElementAt(1, 1, cosf(Convert::DegreesToRadians(angle)));
			out.SetElementAt(1, 0, -sinf(Convert::DegreesToRadians(angle)));
			out.SetElementAt(0, 1, sinf(Convert::DegreesToRadians(angle)));
			out.SetElementAt(2, 2, 1.0f);
		}
		void settranslatemat(Mat3x3& out, const float& x, const float& y)
		{
			out.SetElementAt(0, 0, 1.0f);
			out.SetElementAt(1, 1, 1.0f);
			out.SetElementAt(2, 2, 1.0f);
			out.SetElementAt(2, 0, x);
			out.SetElementAt(2, 1, y);
		}
		void setscalemat(Mat3x3& out, const float& x, const float& y)
		{
			out.SetElementAt(0, 0, x);
			out.SetElementAt(1, 1, y);
			out.SetElementAt(2, 2, 1.0f);
		}
		Vec2D vectoscreen(const Vec2D& vec, const float& cx, const float& cy)
		{
			Vec2D val;
			val.SetX(vec.GetX() + cx);
			val.SetY(cy - vec.GetY());
			return val;
		}
		Vec2D screentovec(const float& x, const float& y, const float& cx, const float& cy)
		{
			Vec2D val;
			val.SetX(x - cx);
			val.SetY(cy - y);

			return val;
		}
		void vecrotate(Vec2D& out, const float& angle)
		{
			Mat3x3 rm;
			setrotationmat(rm, angle);
			vecmul(out, rm);
		}
		void vecscale(Vec2D& out, const float& x, const float& y)
		{
			Mat3x3 sm;
			setscalemat(sm, x, y);
			vecmul(out, sm);
		}
		void vectranslate(Vec2D& out, const float& x, const float& y)
		{
			Mat3x3 tm;
			settranslatemat(tm, x, y);
			vecmul(out, tm);
		}
	}
}
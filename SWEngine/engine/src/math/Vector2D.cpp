#include "../../include/math/Vector2D.h"
#include <math.h>

namespace SWE
{
	namespace Math
	{
		Vector2D::Vector2D() : x(0.0f), y(0.0f), w(1.0f) {}
		Vector2D::Vector2D(const float& x, const float& y) : x(x), y(y), w(1.0f) {}
		Vector2D::Vector2D(const Vector2D& vec)
		{
			x = vec.GetX();
			y = vec.GetY();
			w = vec.GetW();
		}
		Vector2D::~Vector2D() {}
		void Vector2D::SetX(const float& x)
		{
			this->x = x;
		}
		void Vector2D::SetY(const float& y)
		{
			this->y = y;
		}
		void Vector2D::SetW(const float& w)
		{
			this->w = w;
		}
		float Vector2D::GetX() const
		{
			return x;
		}
		float Vector2D::GetY() const
		{
			return y;
		}
		float Vector2D::GetW() const
		{
			return w;
		}

		float Vector2D::GetLength() const
		{
			return sqrtf((x*x) + (y*y));
		}
		Vector2D Vector2D::Normalize() const
		{
			Vector2D ret_val;
			float len = GetLength();
			if (len == 0) return ret_val;
			ret_val.x = x / len;
			ret_val.y = y / len;
			return ret_val;
		}

		Vector2D Vector2D::operator+(const Vector2D& vec)
		{
			Vector2D ret_val;
			ret_val.x = x + vec.x;
			ret_val.y = y + vec.y;
			return ret_val;
		}
		Vector2D Vector2D::operator-(const Vector2D& vec)
		{
			Vector2D ret_val;
			ret_val.x = x - vec.x;
			ret_val.y = y - vec.y;
			return ret_val;
		}
		Vector2D Vector2D::operator*(const float& num)
		{
			Vector2D ret_val;
			ret_val.x = x * num;
			ret_val.y = y * num;
			return ret_val;
		}
		Vector2D Vector2D::operator*(const Matrix3x3& mat)
		{
			Vector2D ret_val;
			ret_val.x = x * mat.GetElementAt(0, 0) + y * mat.GetElementAt(1, 0) + w * mat.GetElementAt(2, 0);
			ret_val.y = x * mat.GetElementAt(0, 1) + y * mat.GetElementAt(1, 1) + w * mat.GetElementAt(2, 1);
			ret_val.w = x * mat.GetElementAt(0, 2) + y * mat.GetElementAt(1, 2) + w * mat.GetElementAt(2, 2);
			return ret_val;
		}
	}
}
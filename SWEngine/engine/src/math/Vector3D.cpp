#include "../../include/math/Vector3D.h"
#include <math.h>

namespace swe
{
	namespace Math
	{
		Vector3D::Vector3D() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		Vector3D::Vector3D(const float& x, const float& y, const float& z) : x(x), y(y), z(z), w(1.0f) {}
		Vector3D::Vector3D(const Vector3D& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}
		Vector3D::~Vector3D() {}

		void Vector3D::SetX(const float& x)
		{
			this->x = x;
		}
		void Vector3D::SetY(const float& y)
		{
			this->y = y;
		}
		void Vector3D::SetZ(const float& z)
		{
			this->z = z;
		}
		void Vector3D::SetW(const float& w)
		{
			this->w = w;
		}
		float Vector3D::GetX() const
		{
			return x;
		}
		float Vector3D::GetY() const
		{
			return y;
		}
		float Vector3D::GetZ() const
		{
			return z;
		}
		float Vector3D::GetW() const
		{
			return w;
		}

		float Vector3D::GetLength() const
		{
			return sqrtf((x*x) + (y*y) + (z*z));
		}
		Vector3D Vector3D::Normalize() const
		{
			Vector3D ret_val;
			float len = GetLength();
			if (len == 0) return ret_val;
			ret_val.x = x / len;
			ret_val.y = y / len;
			ret_val.z = z / len;
			return ret_val;
		}

		float Vector3D::DotProduct(const Vector3D& vec1, const Vector3D& vec2)
		{
			return vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z;
		}
		Vector3D Vector3D::CrossProduct(const Vector3D& vec1, const Vector3D& vec2)
		{
			Vector3D ret_val;
			ret_val.x = (vec1.y * vec2.z) - (vec1.z * vec2.y);
			ret_val.y = (vec1.z * vec2.x) - (vec1.x * vec2.z);
			ret_val.z = (vec1.x * vec2.y) - (vec1.y * vec2.x);
			return ret_val;
		}

		Vector3D Vector3D::operator+(const Vector3D& vec)
		{
			Vector3D ret_val;
			ret_val.x = x + vec.x;
			ret_val.y = y + vec.y;
			ret_val.z = z + vec.z;
			return ret_val;
		}
		void Vector3D::operator+=(const Vector3D& vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
		}
		Vector3D Vector3D::operator-(const Vector3D& vec)
		{
			Vector3D ret_val;
			ret_val.x = x - vec.x;
			ret_val.y = y - vec.y;
			ret_val.z = z - vec.z;
			return ret_val;
		}
		void Vector3D::operator-=(const Vector3D& vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
		}
		Vector3D Vector3D::operator*(const float& num)
		{
			Vector3D ret_val;
			ret_val.x = x * num;
			ret_val.y = y * num;
			ret_val.z = z * num;
			return ret_val;
		}
		void Vector3D::operator*=(const float& num)
		{
			x *= num;
			y *= num;
			z *= num;
		}
		Vector3D Vector3D::operator*(const Matrix4x4& mat)
		{
			Vector3D ret_val;
			ret_val.x = x * mat.GetElementAt(0, 0) + y * mat.GetElementAt(1, 0) + z * mat.GetElementAt(2, 0) + w * mat.GetElementAt(3, 0);
			ret_val.y = x * mat.GetElementAt(0, 1) + y * mat.GetElementAt(1, 1) + z * mat.GetElementAt(2, 1) + w * mat.GetElementAt(3, 1);
			ret_val.z = x * mat.GetElementAt(0, 2) + y * mat.GetElementAt(1, 2) + z * mat.GetElementAt(2, 2) + w * mat.GetElementAt(3, 2);
			ret_val.w = x * mat.GetElementAt(0, 3) + y * mat.GetElementAt(1, 3) + z * mat.GetElementAt(2, 3) + w * mat.GetElementAt(3, 3);
			return ret_val;
		}
		void Vector3D::operator*=(const Matrix4x4& mat)
		{
			Vector3D temp = *this;
			x = temp.x * mat.GetElementAt(0, 0) + temp.y * mat.GetElementAt(1, 0) + temp.z * mat.GetElementAt(2, 0) + temp.w * mat.GetElementAt(3, 0);
			y = temp.x * mat.GetElementAt(0, 1) + temp.y * mat.GetElementAt(1, 1) + temp.z * mat.GetElementAt(2, 1) + temp.w * mat.GetElementAt(3, 1);
			z = temp.x * mat.GetElementAt(0, 2) + temp.y * mat.GetElementAt(1, 2) + temp.z * mat.GetElementAt(2, 2) + temp.w * mat.GetElementAt(3, 2);
			w = temp.x * mat.GetElementAt(0, 3) + temp.y * mat.GetElementAt(1, 3) + temp.z * mat.GetElementAt(2, 3) + temp.w * mat.GetElementAt(3, 3);
		}
	}
}
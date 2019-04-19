#include "../../include/math/Transform2D.h"
#include "../../include/math/Matrix3x3.h"
#include "../../include/math/Converter.h"
#include <math.h>

namespace SWE
{
	namespace Math
	{
		Transform2D::Transform2D() : position() {}
		Transform2D::~Transform2D() {}

		void Transform2D::Translate(const float& x, const float& y)
		{
			Matrix3x3 mat;
			mat.SetElementAt(0, 0, 1);
			mat.SetElementAt(1, 1, 1);
			mat.SetElementAt(2, 0, x);
			mat.SetElementAt(2, 1, y);
			mat.SetElementAt(2, 2, 1);

			position = position * mat;
		}
		void Transform2D::Rotate(const float& angle)
		{
			Matrix3x3 mat;
			mat.SetElementAt(0, 0, cosf(Converter::DegreesToRadians(angle)));
			mat.SetElementAt(0, 1, -sinf(Converter::DegreesToRadians(angle)));
			mat.SetElementAt(1, 0, sinf(Converter::DegreesToRadians(angle)));
			mat.SetElementAt(1, 1, cosf(Converter::DegreesToRadians(angle)));
			mat.SetElementAt(2, 2, 1);
			
			position = position * mat;
		}
		void Transform2D::Scale(const float& x, const float& y)
		{
			Matrix3x3 mat;
			mat.SetElementAt(0, 0, x);
			mat.SetElementAt(1, 1, y);
			mat.SetElementAt(2, 2, 1);

			position = position * mat;
		}
	}
}
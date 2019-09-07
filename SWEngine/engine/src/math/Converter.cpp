#include "../../include/math/Converter.h"

namespace swe
{
	namespace Math
	{
		float Converter::DegreesToRadians(const float& deg)
		{
			return deg * (3.14159f / 180.0f);
		}
	}
}
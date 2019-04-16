#include "../../include/math/Converter.h"

namespace SWEngine
{
	namespace Math
	{
		float Converter::DegreesToRadians(const float& deg)
		{
			return deg * (3.14159f / 180.0f);
		}
	}
}
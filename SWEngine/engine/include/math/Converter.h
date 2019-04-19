#ifndef SWE_CONVERTER
#define SWE_CONVERTER

#include "../Export.h"

namespace SWE
{
	namespace Math
	{
		class SWE_API Converter
		{
		public:
			static float DegreesToRadians(const float& deg);
		};
	}
}

#endif
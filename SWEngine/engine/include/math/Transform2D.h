#ifndef SWE_TRANSFORM2D
#define SWE_TRANSFORM2D

#include "Vector2D.h"
#include "../Export.h"

namespace swe
{
	namespace Math
	{
		class SWE_API Transform2D
		{
		public:
			Transform2D();
			~Transform2D();

			void Translate(const float& x, const float& y);
			void Rotate(const float& angle);
			void Scale(const float& x, const float& y);

		protected:
			Vector2D position;
		};
	}
}

#endif
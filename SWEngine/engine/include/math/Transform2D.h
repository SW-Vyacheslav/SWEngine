#pragma once
#include "Vector2D.h"

namespace SWEngine
{
	namespace Math
	{
		class Transform2D
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
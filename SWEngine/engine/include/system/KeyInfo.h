#ifndef SWE_KEYINFO
#define SWE_KEYINFO

#include "../Export.h"

namespace swe
{
	namespace system
	{
		class SWE_API KeyInfo
		{
		public:
			KeyInfo();
			~KeyInfo();

			bool KeyDown;
			bool KeyUp;
			bool KeyPressed;
		};
	}
}

#endif
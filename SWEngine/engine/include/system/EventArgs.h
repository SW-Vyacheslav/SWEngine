#ifndef SWE_EVENTARGS
#define SWE_EVENTARGS

#include "../Export.h"

namespace swe
{
	namespace system
	{
		class SWE_API EventArgs
		{
		public:
			EventArgs();
			~EventArgs();

			void* engine;
		};
	}
}

#endif
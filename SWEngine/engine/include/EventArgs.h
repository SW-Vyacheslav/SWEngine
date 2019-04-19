#ifndef SWE_EVENTARGS
#define SWE_EVENTARGS

#include "Export.h"

namespace SWE
{
	class SWE_API EventArgs
	{
	public:
		EventArgs();
		~EventArgs();

		void* engine;
	};
}

#endif
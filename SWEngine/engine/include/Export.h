#ifndef SWE_EXPORT
#define SWE_EXPORT 

#include "Config.h"

#ifdef SWE_ALL_EXPORT
	#define SWE_API SWE_API_EXPORT
#else
	#define SWE_API SWE_API_IMPORT
#endif

#endif
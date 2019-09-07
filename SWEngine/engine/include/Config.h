#ifndef SWE_CONFIG
#define SWE_CONFIG

#ifndef UNICODE
	#error UNICODE is not enabled for your compiler!
#endif

#ifdef _WIN32
	#define SWE_SYSTEM_WINDOWS
#else
	#error This operating system is not supported by SWEngine library!
#endif

#ifndef SWE_STATIC
	#ifdef SWE_SYSTEM_WINDOWS
		#define SWE_API_EXPORT __declspec(dllexport) 
		#define SWE_API_IMPORT __declspec(dllimport)
		#ifdef _MSC_VER
			#pragma warning(disable: 4251)
		#endif
	#endif
#else
	#define SWE_API_EXPORT
	#define SWE_API_IMPORT
#endif

namespace swe
{
	typedef signed char Int8;
	typedef unsigned char UInt8;
	typedef signed short Int16;
	typedef unsigned short UInt16;
	typedef signed long Int32;
	typedef unsigned long UInt32;
	typedef signed long long Int64;
	typedef unsigned long long UInt64;
	typedef bool Bool;
	typedef char Char;
	typedef float Float;
	typedef double Double;
}

#endif
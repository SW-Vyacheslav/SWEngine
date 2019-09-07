#include "../../include/system/KeyInfo.h"

namespace swe
{
	namespace system
	{
		KeyInfo::KeyInfo() : KeyDown(false), KeyUp(false), KeyPressed(false) {}
		KeyInfo::~KeyInfo() {}
	}
}
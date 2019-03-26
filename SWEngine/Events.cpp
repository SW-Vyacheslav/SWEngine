#include "Events.h"

namespace SWEngine
{
	namespace Events
	{
		Event::Event(){}

		void Event::operator+=(EventHandler handler)
		{
			handlers.push_back(handler);
		}
		void Event::operator-=(EventHandler handler)
		{
			handlers.remove(handler);
		}
		void Event::operator()(void* sender, EventArgs* args)
		{
			for (EventHandler handler : handlers)
			{
				if (handler) handler(sender, args);
			}
		}
	}
}
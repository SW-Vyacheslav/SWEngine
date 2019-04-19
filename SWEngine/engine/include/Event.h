#ifndef SWE_EVENT
#define SWE_EVENT

#include "EventArgs.h"
#include <list>

#include "Export.h"

namespace SWE
{
	SWE_API typedef void(*EventHandler)(void* sender, EventArgs* args);

	class SWE_API Event
	{
	public:
		Event();
		Event(const Event& e) = delete;
		~Event();

		void operator=(Event e) = delete;
		void operator+=(EventHandler handler);
		void operator-=(EventHandler handler);
		void operator()(void* sender, EventArgs* args);

	private:
		std::list<EventHandler> handlers;
	};
}

#endif
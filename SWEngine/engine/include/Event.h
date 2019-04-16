#pragma once
#include "EventArgs.h"
#include <list>

namespace SWEngine
{
	typedef void(*EventHandler)(void* sender, EventArgs* args);

	class Event
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
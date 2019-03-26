#pragma once
#include <list>

namespace SWEngine
{
	namespace Events
	{
		struct EventArgs
		{
			void* engine;
		};

		using EventHandler = void(*)(void* sender, EventArgs* args);

		class Event
		{
		public:
			Event();
			Event(const Event& e) = delete;

			void operator=(Event e) = delete;
			void operator+=(EventHandler handler);
			void operator-=(EventHandler handler);
			void operator()(void* sender, EventArgs* args);

		private:
			std::list<EventHandler> handlers;
		};
	}
}
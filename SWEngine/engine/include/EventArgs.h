#pragma once

namespace SWEngine
{
	class EventArgs
	{
	public:
		EventArgs();
		~EventArgs();

		void* engine;
	};
}
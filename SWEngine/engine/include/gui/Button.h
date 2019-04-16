#pragma once
#include "UIElement.h"

namespace SWEngine
{
	namespace GUI
	{
		class Button : public UIElement
		{
		public:
			Button(const int& x, const int& y, const int& width, const int& height);
			~Button();

			void UpdateElement(void* engine) override;
		};
	}
}
#ifndef SWE_BUTTON
#define SWE_BUTTON

#include "UIElement.h"

#include "../Export.h"

namespace SWE
{
	namespace GUI
	{
		class SWE_API Button : public UIElement
		{
		public:
			Button(const int& x, const int& y, const int& width, const int& height);
			~Button();

			void UpdateElement(void* engine) override;
		};
	}
}

#endif
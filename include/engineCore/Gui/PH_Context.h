/*
Phobos 3d
September 2012
Copyright (c) 2005-2012 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


#ifndef PH_GUI_CONTEXT_H
#define PH_GUI_CONTEXT_H

#include <Rocket/Core.h>

#include <PH_Node.h>

#include "PH_EngineCoreAPI.h"

#include "Gui/PH_ContextFwd.h"

namespace Phobos
{
	namespace System
	{
		struct InputEvent_s;
	}

	namespace Gui
	{			
		class PH_ENGINE_CORE_API Context_c: public Node_c
		{			
			public:
				static ContextPtr_t Create(const String_t &name, size_t screenWidth, size_t screenHeight);

				Context_c(const String_t &name, size_t screenWidth, size_t screenHeight);
				~Context_c();

				void Update();
				void Render();

				void InputEvent(const System::InputEvent_s &event);

				void ResetMousePosition(short x, short y);

				Rocket::Core::ElementDocument *LoadMouseCursor(const Char_t *path);
				Rocket::Core::ElementDocument *LoadDocument(const Char_t *path);				

			protected:
				Rocket::Core::Context *pclContext;
		};
	}
}

#endif
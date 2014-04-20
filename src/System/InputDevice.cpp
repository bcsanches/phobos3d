/*
Phobos 3d
July 2010
Copyright (c) 2005-2013 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "Phobos/System/InputDevice.h"

void Phobos::System::InputDevice::DispatchEvent(const Phobos::System::InputEvent_s &e)
{	
	for (auto it = m_lstListeners.begin(), end = m_lstListeners.end(); it != end;)
	{
		//advance to next before firing handle
		//so if handler removes itself from the list, the iterator is not invalidated
		auto current = it++;

		current->OnInputEvent(e);
	}
}

PH_DEFINE_LISTENER_PROCS(Phobos::System::InputDevice, InputDeviceListener, m_lstListeners);

/*
Phobos 3d
October 2010
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


#ifndef PH_IPLAYER_COMMAND_PRODUCER_H
#define PH_IPLAYER_COMMAND_PRODUCER_H

/*
	Mouse can be clipped or not to the screen.

	When it is clipped, the cursor is hidden and not visible. Also the input system will try to keep 
	it centered on screen.

	When not clipped its movement is ignored by the input system.

	A dynamic variable (console variable) can be created with the MOUSE_CLIPPED_VAR name and will be monitored
	by the Command Producer.
*/
#define PH_PLAYER_CMD_MOUSE_CLIPPED_VAR "dvMouseClipped"

namespace Phobos
{
	class IPlayerCmd_c
	{
		public:		
	};

	typedef boost::shared_ptr<IPlayerCmd_c> IPlayerCmdPtr_t;

	class IPlayerCommandProducer_c
	{
		public:
			virtual IPlayerCmdPtr_t CreateCmd() = 0;

			virtual void Enable() = 0;
			virtual void Disable() = 0;

			virtual void EnableMouse() = 0;
			virtual void DisableMouse() = 0;
	};
}

#endif

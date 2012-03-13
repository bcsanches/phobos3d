/*
Phobos 3d
May 2010
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

#ifndef PH_CONSOLE_H
#define PH_CONSOLE_H

#include <OgrePrerequisites.h>
#include <OgreRectangle2D.h>

#include <PH_Console.h>

#include "PH_OgreEngineCoreAPI.h"

namespace Phobos
{
	class AxisButton_c;
	class Button_c;
	class OgreConsole_c;
	class Thumb_c;

	PH_DECLARE_NODE_PTR(OgreConsole);	

	class PH_OGRE_ENGINE_CORE_API OgreConsole_c: 
		public Console_c		
	{		
		private:
			// =====================================================
			// PRIVATE TYPES
			// =====================================================
			enum ConsoleFlags_e
			{
				CONSOLE_FLAG_ACTIVE = 0x01,
				CONSOLE_FLAG_IGNORE_FIRST_CHAR = 0x02,
				CONSOLE_TEXT_BUFFER_CHANGED = 0x04,
				CONSOLE_EDIT_BOX_CHANGED = 0x08,
				CONSOLE_UI_MOVED = 0x10,
			};				

		public:
			// =====================================================
			// PUBLIC METHODS
			// =====================================================		
			static OgreConsolePtr_t CreateInstance(void);

		protected:
			// =====================================================
			// PROTECTED METHODS
			// =====================================================
			OgreConsole_c(void);
			~OgreConsole_c(void);

			void OnFixedUpdate();			
			void OnUpdate();
			void OnRenderReady();

			virtual void OnToggleConsole();
			virtual void OnEditBoxChanged();
			virtual void OnTextListChanged();

		private:
			// =====================================================
			// PRIVATE METHODS
			// =====================================================
			void UpdateRenderInfo();
			
		private:
			// =====================================================
			// PRIVATE ATTRIBUTES
			// =====================================================			
			Ogre::OverlayContainer		*pclRect;		
			Ogre::OverlayElement		*pclTextBox;
			Ogre::Overlay				*pclOverlay;
			Ogre::SceneManager			*pclSceneManager;
			Ogre::Camera				*pclCamera;
			Ogre::Overlay				*pclRenderInfoOverlay;

			Float_t						fpHeight;			

			ContextVar_c				varMaterialName;
			ContextVar_c				varShowRenderInfo;			
												
			bool fUIMoved;
			bool fEditBoxChanged;
			bool fTextBufferChanged;
	};	
}

#endif

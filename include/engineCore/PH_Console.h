/*
Phobos 3d
  May 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#ifndef PH_CONSOLE_H
#define PH_CONSOLE_H

#include "PH_CoreModule.h"

#include <boost/circular_buffer.hpp>

#include <OgrePrerequisites.h>
#include <OgreRectangle2D.h>

#include <PH_Context.h>

#include "PH_EngineCoreAPI.h"

namespace Phobos
{
	class Console_c;

	typedef boost::intrusive_ptr<Console_c> ConsolePtr_t;	

	class PH_ENGINE_CORE_API Console_c: public CoreModule_c
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

			class EditBox_c
			{
				public:
					inline EditBox_c(void);		

					inline void AddChar(Char_t);

					inline void RetreatCursor(void);
					inline void AdvanceCursor(void);

					inline void Backspace(void);

					inline void Clear(void);

					inline void SetStr(const String_c &str);

					inline const String_c &GetStr(void) const;

				private:
					// =====================================================
					// PRIVATE ATTRIBUTES
					// =====================================================
					String_c	strStr;

					UInt_t		uCursorPos;

			};

		public:
			// =====================================================
			// PUBLIC METHODS
			// =====================================================		
			static ConsolePtr_t &CreateInstance(void);
			static ConsolePtr_t &GetInstance(void);
			static void ReleaseInstance(void);		

			void Execute(const String_c &cmdLine);
			void ExecuteFromFile(const String_c &fileName);

			inline bool IsActive(void) const;

			inline void AddContextVar(class ContextVar_c &var);
			inline void AddContextCmd(class ContextCmd_c &cmd);

			void Print(const String_c &text);			

		protected:
			// =====================================================
			// PROTECTED METHODS
			// =====================================================
			Console_c(void);
			~Console_c(void);

			void OnFixedUpdate();
			void OnInit();
			void OnUpdate();
			void OnRenderReady();

		private:
			// =====================================================
			// PRIVATE METHODS
			// =====================================================
			void CreateDefaultCmds();
				
			void OnChar(Char_t ch);
			void OnEnter(void);		

			void OnPreviousCommand(void);
			void OnNextCommand(void);

			void RegisterDefaultCmds(void);

			void ToggleConsole(void);

			//void LogProc(IM_UInt8_t level, const IM_SInt8_t *text);

			void AddToHistory(const String_c &str);

			bool GetPreviousCommand(String_c &out);
			bool GetNextCommand(String_c &out);

			void UpdateRenderInfo();

		private:
			// =====================================================
			// PRIVATE ATTRIBUTES
			// =====================================================
			//FIXME
			//InputManagerPtr_t			ipInputManager;			

			EditBox_c					clEditBox;			

			Context_c					clContext;

			Ogre::OverlayContainer		*pclRect;		
			Ogre::OverlayElement		*pclTextBox;
			Ogre::Overlay				*pclOverlay;
			Ogre::SceneManager			*pclSceneManager;
			Ogre::Camera				*pclCamera;
			Ogre::Overlay				*pclRenderInfoOverlay;

			Float_t						fpHeight;

			typedef boost::circular_buffer<String_c> TextList_t;
			TextList_t	lstText;
			TextList_t	lstHistory;

			TextList_t::iterator		itPrevCmd;

			ContextVar_c				varMaterialName;
			ContextVar_c				varShowRenderInfo;
			ContextCmd_c				cmdLsO;
			ContextCmd_c				cmdCdO;

			bool fActive;
			bool fIgnoreFirstChar;
			bool fTextBufferChanged;
			bool fEditBoxChanged;
			bool fUIMoved;			
					
		private:
			// =====================================================
			// STATIC PRIVATE METHODS
			// =====================================================		
			//IM_ErrorHandler_t InputManagerListenerProc(IM_Handler_t input, IM_Handler_t param);

			//IM_ErrorHandler_t InputDeviceKeyboardListenerProc(IM_Handler_t input, IM_Handler_t param);		

		private:
			// =====================================================
			// STATIC PRIVATE ATTRIBUTES
			// =====================================================
			static ConsolePtr_t ipInstance_gl;

	};

	// =====================================================
	// INLINE IMPLEMENTATION
	// =====================================================

	inline bool Console_c::IsActive(void) const
	{		
		return fActive;
	}

	inline void Console_c::AddContextVar(class ContextVar_c &var)
	{
		return clContext.AddContextVar(var);
	}

	inline void Console_c::AddContextCmd(class ContextCmd_c &cmd)
	{
		return clContext.AddContextCmd(cmd);
	}
}

#endif

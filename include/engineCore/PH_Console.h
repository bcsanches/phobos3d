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

#include "PH_CoreModule.h"

#include <boost/circular_buffer.hpp>

#include <OgrePrerequisites.h>
#include <OgreRectangle2D.h>

#include <PH_Context.h>
#include <PH_InputManager.h>
#include <PH_InputMapper.h>
#include <PH_Log.h>

#include "PH_EngineCoreAPI.h"

namespace Phobos
{
	class AxisButton_c;
	class Button_c;
	class Console_c;
	class Thumb_c;

	PH_DECLARE_NODE_PTR(Console);	

	class PH_ENGINE_CORE_API Console_c: 
		public CoreModule_c, 
		public IContext_c,
		private LogListener_c, 
		private InputManagerListener_c, 
		private InputDeviceListener_c
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

			inline void AddContextVar(ContextVar_c &var);
			inline void AddContextCmd(ContextCmd_c &cmd);
			inline void RemoveContextCmd(ContextCmd_c &cmd);

			inline const ContextVar_c &GetContextVar(const String_c &name) const;
			inline const ContextVar_c *TryGetContextVar(const String_c &name) const;

			void Print(const String_c &text);		

		protected:
			// =====================================================
			// PROTECTED METHODS
			// =====================================================
			Console_c(void);
			~Console_c(void);

			void OnFixedUpdate();
			//void OnInit();
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

			void AddToHistory(const String_c &str);

			bool GetPreviousCommand(String_c &out);
			bool GetNextCommand(String_c &out);

			void UpdateRenderInfo();

			//Log message handler
			void Message(const String_c &message);

			void InputManagerEvent(const InputManagerEvent_s &event);
			void InputEvent(const InputEvent_s &event);

			void CmdCd(const StringVector_t &args, Context_c &);
			void CmdLs(const StringVector_t &args, Context_c &);
			void CmdDumpTable(const StringVector_t &args, Context_c &);

		private:
			// =====================================================
			// PRIVATE ATTRIBUTES
			// =====================================================
			//FIXME
			//InputManagerPtr_t			ipInputManager;			

			EditBox_c					clEditBox;			

			Context_c					clContext;

			InputMapperPtr_t			ipInputMapper;

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
			ContextCmd_c				cmdLs;
			ContextCmd_c				cmdCd;
			ContextCmd_c				cmdDumpTable;

			String_c					strCurrentNodePathName;

			bool fActive;
			bool fIgnoreFirstChar;
			bool fIgnoredLastChar;
			bool fTextBufferChanged;
			bool fEditBoxChanged;
			bool fUIMoved;							

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

	inline void Console_c::AddContextVar(ContextVar_c &var)
	{
		return clContext.AddContextVar(var);
	}

	inline void Console_c::AddContextCmd(ContextCmd_c &cmd)
	{
		return clContext.AddContextCmd(cmd);
	}	

	inline void Console_c::RemoveContextCmd(ContextCmd_c &cmd)
	{
		return clContext.RemoveContextCmd(cmd);
	}

	inline const ContextVar_c &Console_c::GetContextVar(const String_c &name) const
	{
		return clContext.GetContextVar(name);
	}

	inline const ContextVar_c *Console_c::TryGetContextVar(const String_c &name) const
	{
		return clContext.TryGetContextVar(name);
	}
}

#endif

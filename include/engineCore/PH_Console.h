/*
Phobos 3d
February 2012
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

#include "PH_CoreModule.h"

#include <boost/circular_buffer.hpp>

#include <PH_Context.h>
#include <PH_Log.h>

#include "PH_IInputHandler.h"

namespace Phobos
{
	class Console_c;

	namespace
	{
		struct InputEvent_s;
	}

	PH_DECLARE_NODE_PTR(Console);	

	class PH_ENGINE_CORE_API Console_c: 
		public CoreModule_c, 
		public IContext_c,
		private LogListener_c,
		public IInputHandler_c
	{
		private:
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

		protected:
			typedef boost::circular_buffer<String_c> TextList_t;

		public:
			static Console_c &GetInstance(void);			
			static void ReleaseInstance(void);

			void Execute(const String_c &cmdLine);
			void ExecuteFromFile(const String_c &fileName);

			inline void AddContextVar(ContextVar_c &var);
			inline void AddContextCmd(ContextCmd_c &cmd);
			inline void RemoveContextCmd(ContextCmd_c &cmd);			

			inline const ContextVar_c &GetContextVar(const String_c &name) const;
			inline const ContextVar_c *TryGetContextVar(const String_c &name) const;
			inline ContextVar_c *TryGetContextVar(const String_c &name);

			void Print(const String_c &text);

			inline bool IsActive(void) const;

			void ToggleConsole(void);

			void FlushCommandBuffer();
			
			bool HandleInputEvent(const System::InputEvent_s &event);

		protected:
			Console_c(const String_c &name, UInt32_t flags = 0);
			virtual ~Console_c();

			virtual void OnToggleConsole() = 0;
			virtual void OnEditBoxChanged() = 0;
			virtual void OnTextListChanged() = 0;			

			inline TextList_t::const_iterator ListTextBegin() const;
			inline TextList_t::const_iterator ListTextEnd() const;

			const String_c &EditBoxStr() const;

		protected:
			static void UpdateInstance(ConsolePtr_t console);

		private:
			void CmdCd(const StringVector_t &args, Context_c &);
			void CmdLs(const StringVector_t &args, Context_c &);
			void CmdDumpTable(const StringVector_t &args, Context_c &);			

			void OnChar(Char_t ch);
			void OnEnter(void);		

			void OnPreviousCommand(void);
			void OnNextCommand(void);

			void AddToHistory(const String_c &str);

			bool GetPreviousCommand(String_c &out);
			bool GetNextCommand(String_c &out);

			//Log message handler
			void Message(const String_c &message);					

			void QueueCommand(const String_c &cmd);

		private:			
			Context_c					clContext;

			ContextCmd_c				cmdLs;
			ContextCmd_c				cmdCd;
			ContextCmd_c				cmdDumpTable;			
			
			String_c					strCurrentNodePathName;			

			EditBox_c					clEditBox;
			std::stringstream			clCommandBuffer;
			
			TextList_t	lstText;
			TextList_t	lstHistory;

			TextList_t::iterator		itPrevCmd;
			
			bool fActive;

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

	inline ContextVar_c *Console_c::TryGetContextVar(const String_c &name)
	{
		return clContext.TryGetContextVar(name);
	}

	inline const ContextVar_c *Console_c::TryGetContextVar(const String_c &name) const
	{
		return clContext.TryGetContextVar(name);
	}

	inline Console_c::TextList_t::const_iterator Console_c::ListTextBegin() const
	{
		return lstText.begin();
	}

	inline Console_c::TextList_t::const_iterator Console_c::ListTextEnd() const
	{
		return lstText.end();
	}	
}

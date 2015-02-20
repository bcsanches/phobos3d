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

#ifndef PH_ENGINE_CONSOLE_H
#define PH_ENGINE_CONSOLE_H

#include "Phobos/Engine/Module.h"
#include "Phobos/Engine/IInputHandler.h"

#include <boost/circular_buffer.hpp>

#include <Phobos/Shell/Context.h>
#include <Phobos/Log.h>

namespace Phobos
{
	namespace Engine
	{
		class Console;

		namespace
		{
			struct InputEvent_s;
		}

		PH_DECLARE_NODE_PTR(Console);	

		class PH_ENGINE_API Console: 
			public Module, 
			public Shell::IContext,
			private Log::Listener,
			public IInputHandler
		{
			private:
				class EditBox
				{
					public:
						inline EditBox(void);		

						inline void AddChar(Char_t);

						inline void RetreatCursor(void);
						inline void AdvanceCursor(void);

						inline void Backspace(void);

						inline void Clear(void);

						inline void SetStr(const String_t &str);

						inline const String_t &GetStr(void) const;

					private:
						// =====================================================
						// PRIVATE ATTRIBUTES
						// =====================================================
						String_t	m_strStr;

						UInt_t		m_uCursorPos;

				};

			protected:
				typedef boost::circular_buffer<String_t> TextList_t;

			public:
				//
				//
				//This is the default do nothing console, so you may wish to inherit and replace this class
				//
				static std::unique_ptr<Phobos::Engine::Module> CreateInstance(const Phobos::String_t &name);

				static Console &GetInstance(void);							

				virtual ~Console();

				void Execute(const String_t &cmdLine);
				void ExecuteFromFile(const String_t &fileName);

				virtual void AddContextVariable(Shell::Variable &var) override;
				virtual void AddContextCommand(Shell::Command &cmd) override;
				virtual void RemoveContextCommand(Shell::Command &cmd) override;

				virtual const Shell::Variable &GetContextVariable(const String_t &name) const override;
				virtual const Shell::Variable *TryGetContextVariable(const String_t &name) const override;				

				void Print(const String_t &text);

				inline bool IsActive(void) const;

				void ToggleConsole(void);

				void FlushCommandBuffer();
			
				bool HandleInputEvent(const System::InputEvent_s &event);

			protected:
				Console(const String_t &name, UInt32_t flags = 0);				

				virtual void OnToggleConsole() { };
				virtual void OnEditBoxChanged() { };
				virtual void OnTextListChanged() { };

				inline TextList_t::const_iterator ListTextBegin() const;
				inline TextList_t::const_iterator ListTextEnd() const;

				const String_t &EditBoxStr() const;

			protected:
				static void UpdateInstance(ConsolePtr_t console);

			private:
				void CmdCd(const Shell::StringVector_t &args, Shell::Context &);
				void CmdLs(const Shell::StringVector_t &args, Shell::Context &);
				void CmdDumpTable(const Shell::StringVector_t &args, Shell::Context &);			

				void OnChar(Char_t ch);
				void OnEnter(void);		

				void OnPreviousCommand(void);
				void OnNextCommand(void);

				void AddToHistory(const String_t &str);

				bool GetPreviousCommand(String_t &out);
				bool GetNextCommand(String_t &out);

				//Log message handler
				virtual void OnLogMessage(const String_t &message) override;

				void QueueCommand(const String_t &cmd);

			private:			
				Shell::Context				m_clContext;

				Shell::Command				m_cmdLs;
				Shell::Command				m_cmdCd;
				Shell::Command				m_cmdDumpTable;			
			
				String_t					m_strCurrentNodePathName;			

				EditBox						m_clEditBox;
				std::stringstream			m_clCommandBuffer;
			
				TextList_t	m_lstText;
				TextList_t	m_lstHistory;

				TextList_t::iterator		m_itPrevCmd;
			
				bool m_fActive;
		};

		// =====================================================
		// INLINE IMPLEMENTATION
		// =====================================================

		inline bool Console::IsActive(void) const
		{		
			return m_fActive;
		}		

		inline Console::TextList_t::const_iterator Console::ListTextBegin() const
		{
			return m_lstText.begin();
		}

		inline Console::TextList_t::const_iterator Console::ListTextEnd() const
		{
			return m_lstText.end();
		}	
	}
}

#endif
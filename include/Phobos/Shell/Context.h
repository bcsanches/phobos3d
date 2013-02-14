/*
Phobos 3d
March 2010
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

#ifndef PH_CONTEXT_H
#define PH_CONTEXT_H

#include "Phobos/Shell/Command.h"
#include "Phobos/Shell/Variable.h"
#include "Phobos/Shell/IContext.h"
#include "Phobos/BaseAPI.h"
#include "Phobos/String.h"

namespace Phobos
{
	namespace Shell
	{
		/**

			\ingroup context

			This class take care of a single context. With this class the user can
			create dynamic cmds and variables and execute those cmds.

			The PH_Context_c create those default cmds:
				- createVar: used to create variables (type create var for help).


		*/

		class PH_BASE_API Context: public IContext
		{
			public:
				// =====================================================
				// PUBLIC METHODS
				// =====================================================
				Context(void);

				virtual void AddContextVariable(Variable &var) override;

				/**
					This method sets a variable value.

					If the variable is not found, an exception will be throw.

					If the value cannot be set, an exception will be throw.
				*/
				void SetContextVariableValue(const String_t &name, const String_t &value);

				void RemoveContextVariable(Variable &var);

				virtual void AddContextCommand(Command &cmd) override;
				virtual void RemoveContextCommand(Command &cmd) override;

				virtual const Variable &GetContextVariable(const String_t &name) const override;
				virtual const Variable *TryGetContextVariable(const String_t &name) const override;
				Variable *TryGetContextVariable(const String_t &name);

				virtual void Execute(const String_t &cmdBuffer) override;
				void ExecuteFromFile(const String_t &fileName);

			private:
				// =====================================================
				// PRIVATE METHODS
				// =====================================================			
				const Command *TryGetContextCommand(const String_t &name) const;
				Command *TryGetContextCommand(const String_t &name);

				bool GetCmdLine(String_t &dest, const String_t &src, size_t &currentPos);
				bool ParseCmdLine(StringVector_t &args, const String_t &cmdLine);
				void ParseCmdParam(String_t &result, const String_t &param);

				void ExecuteCmdLine(StringVector_t &args);

				// =====================================================
				// PRIVATE ATRIBUTES
				// =====================================================
				typedef boost::intrusive::set<Command, boost::intrusive::constant_time_size<false> > CommandSet_t;
				typedef boost::intrusive::set<Variable, boost::intrusive::constant_time_size<false> > VariableSet_t;
				CommandSet_t m_setCommands;
				VariableSet_t m_setVariables;

			private:
				// =====================================================
				// STATIC PRIVATE METHODS
				// =====================================================
				void CmdEcho(const StringVector_t &args, Context &);
				void CmdListCmds(const StringVector_t &args, Context &);
				void CmdListVars(const StringVector_t &args, Context &);
				void CmdSet(const StringVector_t &args, Context &);
				void CmdIf(const StringVector_t &args, Context &);

				Command m_cmdHelp;
				Command m_cmdEcho;
				Command m_cmdListCmds;
				Command m_cmdListVars;
				Command m_cmdSet;
				Command m_cmdIf;

				Variable		m_varDebugWithDebugLib;
				Variable		m_varDebugWithReleaseLib;
				Variable		m_varReleaseLib;
				Variable		m_varDebug;
				Variable		m_varRelease;
				Variable		m_varLinux;
				Variable		m_varLinuxDebug;
				Variable		m_varLinuxRelease;
				Variable		m_varWindows;
				Variable		m_varWindowsDebug;
				Variable		m_varWindowsRelease;
				Variable		m_varSDL;
				Variable		m_varSDLDebug;
				Variable		m_varSDLRelease;
		};

		// =====================================================
		// INLINE IMPLEMENTATION
		// =====================================================
	}
}

#endif

/*
Phobos 3d
  March 2010

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

#ifndef PH_CONTEXT_H
#define PH_CONTEXT_H

#include "PH_ContextCmd.h"
#include "PH_ContextVar.h"
#include "PH_IContext.h"
#include "PH_KernelAPI.h"
#include "PH_String.h"

namespace Phobos
{
	/**

		\ingroup context

		This class take care of a single context. With this class the user can
		create dynamic cmds and variables and execute those cmds.

		The PH_Context_c create those default cmds:
			- createVar: used to create variables (type create var for help).


	*/

	class PH_KERNEL_API Context_c: public IContext_c
	{
		public:
			// =====================================================
			// PUBLIC METHODS
			// =====================================================
			Context_c(void);

			void AddContextVar(class ContextVar_c &var);

			/**
				This method sets a variable value.

				If the variable is not found, an exception will be throw.

				If the value cannot be set, an exception will be throw.
			*/
			void SetContextVarValue(const String_c &name, const String_c &value);

			void RemoveContextVar(ContextVar_c &var);

			void AddContextCmd(ContextCmd_c &cmd);
			void RemoveContextCmd(ContextCmd_c &cmd);

			const ContextVar_c &GetContextVar(const String_c &name) const;
			const ContextVar_c *TryGetContextVar(const String_c &name) const;

			void Execute(const String_c &cmdBuffer);
			void ExecuteFromFile(const String_c &fileName);

		private:
			// =====================================================
			// PRIVATE METHODS
			// =====================================================
			ContextVar_c *TryGetContextVar(const String_c &name);
			const ContextCmd_c *TryGetContextCmd(const String_c &name) const;
			ContextCmd_c *TryGetContextCmd(const String_c &name);

			bool GetCmdLine(String_c &dest, const String_c &src, size_t &currentPos);
			bool ParseCmdLine(StringVector_t &args, const String_c &cmdLine);
			void ParseCmdParam(String_c &result, const String_c &param);

			void ExecuteCmdLine(StringVector_t &args);

			// =====================================================
			// PRIVATE ATRIBUTES
			// =====================================================
			typedef boost::intrusive::set<ContextCmd_c, boost::intrusive::constant_time_size<false> > ContextCmdSet_t;
			typedef boost::intrusive::set<ContextVar_c, boost::intrusive::constant_time_size<false> > ContextVarSet_t;
			ContextCmdSet_t setCommands;
			ContextVarSet_t setVariables;

		private:
			// =====================================================
			// STATIC PRIVATE METHODS
			// =====================================================
			void CmdEcho(const StringVector_t &args, Context_c &);
			void CmdListCmds(const StringVector_t &args, Context_c &);
			void CmdListVars(const StringVector_t &args, Context_c &);
			void CmdSet(const StringVector_t &args, Context_c &);
			void CmdIf(const StringVector_t &args, Context_c &);

            ContextCmd_c cmdHelp;
			ContextCmd_c cmdEcho;
			ContextCmd_c cmdListCmds;
			ContextCmd_c cmdListVars;
			ContextCmd_c cmdSet;
			ContextCmd_c cmdIf;

			ContextVar_c	varDebug;
			ContextVar_c	varRelease;
	};

	// =====================================================
	// INLINE IMPLEMENTATION
	// =====================================================
}

#endif

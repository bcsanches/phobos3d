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

#include "PH_Context.h"
#include "PH_ContextUtils.h"

#include <sstream>

#include "PH_Error.h"
#include "PH_Exception.h"
#include "PH_Kernel.h"

using namespace std;

namespace Phobos
{

	struct ContextItemComp_s
	{
		bool operator()(const String_c &lhs, const ContextItem_c &rhs) const
		{
			return lhs.compare(rhs.GetName()) < 0;
		}

		bool operator()(const ContextItem_c &lhs, const String_c &rhs) const
		{
			return lhs.GetName().compare(rhs) < 0;
		}
	};

	template <typename T>
	static inline typename T::const_pointer TryGetItem(T &list, const String_c &name)
	{
		T::const_iterator it = list.find(name, ContextItemComp_s());
		if(it == list.end())
			return NULL;

		return &(*it);
	}	

	template <typename T, typename Y>
	static inline void AddContextItem(T &list, Y &item, const char *objectName, const char *module)
	{
		const Y *other = TryGetItem(list, item.GetName());
		if(other != NULL)
		{
			stringstream str;
			str << objectName << ' ' << item.GetName() << " already exists";
			PH_RAISE(OBJECT_ALREADY_EXISTS_EXCEPTION, module, str.str());
		}
		list.insert(item);
	}

	// =====================================================
	// 
	// =====================================================

	Context_c::Context_c(void):
		cmdEcho("echo"),
		cmdListCmds("listCmds"),
		cmdListVars("listVars"),
		cmdSet("set"),
		cmdIf("if"),
	#ifdef PH_DEBUG
		varDebug("dvDebug", "true"),
		varRelease("dvRelease", "false")
	#else
		varDebug("dvDebug", "false"),
		varRelease("dvRelease", "true")
	#endif
	{
		cmdEcho.SetProc(PH_CONTEXT_CMD_BIND(&Context_c::CmdEcho, this));
		cmdListCmds.SetProc(PH_CONTEXT_CMD_BIND(&Context_c::CmdListCmds, this));
		cmdListVars.SetProc(PH_CONTEXT_CMD_BIND(&Context_c::CmdListVars, this));
		cmdSet.SetProc(PH_CONTEXT_CMD_BIND(&Context_c::CmdSet, this));
		cmdIf.SetProc(PH_CONTEXT_CMD_BIND(&Context_c::CmdIf, this));

		this->AddContextCmd(cmdEcho);
		this->AddContextCmd(cmdListCmds);
		this->AddContextCmd(cmdListVars);
		this->AddContextCmd(cmdSet);
		this->AddContextCmd(cmdIf);

		this->AddContextVar(varDebug);
		this->AddContextVar(varRelease);		
	}	

	void Context_c::AddContextVar(ContextVar_c &var)
	{
		AddContextItem(setVariables, var, "Variable", "Context_c::AddContextVar");		
	}

	void Context_c::RemoveContextVar(ContextVar_c &var)
	{
		var.unlink();
	}

	void Context_c::SetContextVarValue(const String_c &name, const String_c &value)
	{
		ContextVar_c &var(const_cast<ContextVar_c&>(this->GetContextVar(name)));
	
		var.SetValue(value);		
	}

	const ContextVar_c &Context_c::GetContextVar(const String_c &name) const
	{
		const ContextVar_c *p = this->TryGetContextVar(name);
		if(p == NULL)
		{
			std::stringstream str;
			str << "Variable " << name << " not found.";
			PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "&Context_c::GetContextVar", str.str());
		}
		
		return *p;
	}

	const ContextVar_c *Context_c::TryGetContextVar(const String_c &name) const
	{
		return TryGetItem(setVariables, name);
	}

	ContextVar_c *Context_c::TryGetContextVar(const String_c &name)
	{
		return const_cast<ContextVar_c *>(static_cast<const Context_c *>(this)->TryGetContextVar(name));
	}
			
	const ContextCmd_c *Context_c::TryGetContextCmd(const String_c &name) const
	{
		return TryGetItem(setCommands, name);		
	}

	ContextCmd_c *Context_c::TryGetContextCmd(const String_c &name)
	{
		return const_cast<ContextCmd_c *>(static_cast<const Context_c *>(this)->TryGetContextCmd(name));
	}

	void Context_c::AddContextCmd(ContextCmd_c &cmd)
	{
		AddContextItem(setCommands, cmd, "Command", "Context_c::AddContextCmd");		
	}

	void Context_c::RemoveContextCmd(ContextCmd_c &cmd)
	{
		cmd.unlink();
	}

	bool Context_c::GetCmdLine(String_c &dest, const String_c &src, size_t &currentPos)
	{
		size_t len = src.length();
		size_t pos;	
		Char_t ch;
		size_t quotes;
		Size_t size;		

	BEGIN:
		pos = currentPos;		

		//We clear it for security on start, because sometimes we dont store anything there
		dest.clear();

		if(pos >= len)
			return(false);

		bool comment = false;
		for(size = 0, quotes = 0;pos < len; ++pos, ++size)
		{
			ch = src[pos];

			//Maybe we are at a comments begin
			if((ch == '/' && (pos  + 1 < len)))
			{
				//Is this a comment?
				if(src[pos + 1] == '/')
				{
					//If we are at the beginning, lets go to the end of the line and start it again
					if(pos == currentPos)
					{
						for(;;)
						{
							++pos;

							//we reached the end of the buffer?
							if(pos >= len)						
								return(false);				

							//we reached the end of line?
							if(src[pos] == '\n')
							{
								//lets start again
								currentPos = pos + 1;
								goto BEGIN;
							}
						}
					}
					//If we are not at the begin, we finish the cmd line
					else		
					{
						comment = true;
						break;
					}
				}
			}
			//Counting the quotes num
			if(ch == '\"')
				++quotes;
			//If the ; is inside quoetes, we ignore
			else if((ch == ';') && !(quotes & 1))
				break;
			//If dont check quotes here, so we can end a line with: "aaaaaa\n 
			else if(ch == '\n')
				break;
		}

		//now we need to skip invalid chars at the start of the cmd line
		pos = currentPos;
		ch = src[pos];
		while(((ch == '\t') || (ch == ' ')) && (size > 0))
		{
			++pos;
			--size;
			ch = src[pos];
		}

		//the cmd is empty?
		if(size == 0)
		{
			//we reached the end of the buffer?
			if(pos >= len)
				return(false);
			else
			{
				//We just reached a line end, so start it again
				currentPos = pos+1;
				goto BEGIN;
			}
		}

		//copying the result.
		//dest.SetSub(&src, pos, size);
		dest.assign(src, pos, size);

		//saving the new position
		currentPos = pos + (comment ? size : size + 1);

		return(true);
	}

	void Context_c::ParseCmdParam(String_c &result, const String_c &param)
	{
		PH_ASSERT(!param.empty());

		size_t len = param.length();
		Char_t ch = param[0];

		if((ch == '$') && (len > 1))
		{		
			ch = param[1];

			//getting the real value
			//result.Set(param.GetStr() + 1);
			result.assign(param, 1, len);

			//If the second char isnot a $, we got a var name, else we got $$, so copy just one $ 
			//we copied the name with just one $ on the line above (we need this name to search for the var too)
			if(ch != '$')		
			{
				ContextVar_c *var = this->TryGetContextVar(result);
				if(var == NULL)
				{
					stringstream str;
					str << "[Context_c::ParseCmdParam] Error: cant find variable " << result << endl;
					Kernel_c::GetInstance().LogMessage(str.str());
					return;
				}
				else
					result = var->GetValue();
			}
		}
		else	
			result = param;		
	}

	bool Context_c::ParseCmdLine(StringVector_t &args, const String_c &cmdLine)
	{	
		size_t pos;	
		size_t start;
		size_t dataSize;	
		Char_t ch;

		String_c tempStr;
		String_c param;

		args.clear();

		const Char_t *cmdLineStr = cmdLine.c_str();

		pos = 0;
		dataSize = 0;	
		for(;;)
		{
			ch = cmdLineStr[pos];
			while((ch != '\0') && ((ch == '\t') || (ch == ' ') || (ch == '\n') || (ch == '\r')))
			{
				++pos;
				ch = cmdLineStr[pos];
			}

			//end of cmd Line?
			if(ch == '\0')		
				return(!args.empty());					

			if(ch == '\"')
			{
				//skip the first quote
				start = ++pos;
				ch = cmdLineStr[pos];

				//lets try to find second quote
				while((ch != '\0') && (ch != '\"'))
				{
					++dataSize;
					++pos;
					ch = cmdLineStr[pos];
				}

				//Error, quotes not closed
				if(ch == '\0')
				{
					//FIXME print a error message here
					return(false);
				}
				else
				{
					//skip second quote
					++pos;
				}
			}
			else
			{
				start = pos;
				while((ch != '\0') && (ch != '\n') && (ch != '\t') && (ch != ' ') && (ch != '\r'))
				{
					++dataSize;
					++pos;
					ch = cmdLineStr[pos];
				}
			}

			//creating a new entry
			//tempStr.SetSub(cmdLine, start, dataSize);		
			tempStr.assign(cmdLine, start, dataSize);
			this->ParseCmdParam(param, tempStr);

			args.push_back(param);
			dataSize = 0;
		}
	}

	void Context_c::ExecuteCmdLine(StringVector_t &args)
	{		
		PH_ASSERT(!args.empty());

		const String_c	&cmdName = args[0];
		ContextCmd_c		*cmd = this->TryGetContextCmd(cmdName);
		if(cmd == NULL)
		{		
			stringstream stream;
			stream << "[Context_c::ExecuteCmdLine] Command " << cmdName << " not found" << endl;
			Kernel_c::GetInstance().LogMessage(stream.str());			
		}

		return(cmd->Execute(args, *this));
	}

	/**

		Called to execute the buffer. This methods parses cmdBuffer and executes any commands
		that it may find. The cmdBuffer can be composed of several cmdLines.

		To get errors from commands, they should push their errors on stack (IM_ErrorManager_c).

		\param cmdBuffer string with commands to be executed.
		\param contextParam parameter that is stored on IM_ContextStringContainer_c.

	*/
	void Context_c::Execute(const String_c &cmdBuffer)
	{
		size_t			currentPos = 0;
		String_c		cmdLine;
		StringVector_t	args;

		for(;;)
		{
			//getting a cmd line from the buffer
			if(!this->GetCmdLine(cmdLine, cmdBuffer, currentPos))
			{
				//the GetCmdLine error just indicates that the buffer dont have valid cmd line.
				break;
			}

			//build the the args for the command
			if(!this->ParseCmdLine(args, cmdLine))
			{
				//this error is simple ignored.
				continue;
			}

			//lets call the cmd
			this->ExecuteCmdLine(args);			
		}
	}


	// =====================================================
	// COMMANDS
	// =====================================================

	void Context_c::CmdEcho(const StringVector_t &args, Context_c &)
	{	
		size_t sz = args.size();

		stringstream stream;
		if(1 == sz)
		{
			stream << "[DCmdEcho] Error: insuficient parameters" << endl;
			stream << "\tUsage: echo <param1> [<paramn>]" << endl;
			stream << "\tExample: echo hallo $varName\n" << endl;			
		}
		else
		{		
			for(size_t i = 1; i < sz; ++i)
			{			
				if(i > 1)
				{
					stream << ' ';
				}

				stream << args[i];			
			}
		}		
		Kernel_c::GetInstance().LogMessage(stream.str());
	}

	void Context_c::CmdListCmds(const StringVector_t &args, Context_c &)
	{
		//lstCommands.ForEachItem(PrintNodesNameProc);	

		//return(IM_SUCCESS);
	}

	void Context_c::CmdListVars(const StringVector_t &args, Context_c &)
	{	
		//lstVariables.ForEachItem(PrintNodesNameProc);

		//return(IM_SUCCESS);
	}

	void Context_c::CmdSet(const StringVector_t &args, Context_c &)
	{
		stringstream stream;
		if(args.size() < 3)
		{
			stream	<< "[CmdSet] Error: insuficient parameters" << endl
					<< "[CmdSet] Usage: set <varName> <varValue>" << endl
					<< "\tExample: set MyPath \"blabla\"" << endl;			
		}
		else
		{		
			ContextVar_c *var = this->TryGetContextVar(args[1]);
			if(var)
			{
				try
				{
					var->SetValue(args[2]);

					return;
				}
				catch(Exception_c &e)
				{
					stream << "[CmdSet] Error: cant set variable " << var->GetName() << " value to " << args[2] << ", message: " << e.what() << endl;					
				}
			}	
			else
			{
				stream << "[CmdSet] Error: variable " << args[1] << " not found" << endl;				
			}
		}

		Kernel_c::GetInstance().LogMessage(stream.str());
	}


	void Context_c::CmdIf(const StringVector_t &args, Context_c &)
	{
		size_t sz = args.size();
		
		if(sz < 3)
		{
			stringstream stream;
			stream << "[CmdIf] Insuficient parameters, usage: if <cond> <expr> [expr2]" << endl;
			Kernel_c::GetInstance().LogMessage(stream.str());
		}
		else
		{
			const String_c &expr = args[1];
			const String_c *result = NULL;
			
			if(StringToBoolean(expr))
				result = &args[2];
			else
				result = (sz == 4) ? &args[3] : NULL;

			if(result)
			{		
				this->Execute(*result);		
			}
		}
	}
}
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

#include "Phobos/Shell/Context.h"
#include "Phobos/Shell/Utils.h"

#include "Phobos/Error.h"
#include "Phobos/Exception.h"
#include "Phobos/Log.h"

#include <iostream>
#include <sstream>

using namespace std;

namespace
{
	using namespace Phobos;

	struct ItemComp_s
	{
		bool operator()(const String_t &lhs, const Shell::Item &rhs) const
		{
			return lhs.compare(rhs.GetName()) < 0;
		}

		bool operator()(const Shell::Item &lhs, const String_t &rhs) const
		{
			return lhs.GetName().compare(rhs) < 0;
		}
	};

	template <typename T>
	static inline typename T::const_pointer TryGetItem(T &list, const String_t &name)
	{
	    typename T::const_iterator it = list.find(name, ItemComp_s());
		if(it == list.end())
			return NULL;

		return &(*it);
	}

	template <typename T, typename Y>
	static inline void AddItem(T &list, Y &item, const char *objectName, const char *module)
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
}

// =====================================================
//
// =====================================================

Phobos::Shell::Context::Context(void):
    m_cmdHelp("help"),
	m_cmdEcho("echo"),
	m_cmdListCmds("listCmds"),
	m_cmdListVars("listVars"),
	m_cmdSet("set"),
	m_cmdIf("if"),
#ifdef PH_RELEASE_LIBS
	m_varDebugWithDebugLib("dvDebugWithDebugLib", "false"),
	m_varDebugWithReleaseLib("dvDebugWithReleaseLib", "false"),
	m_varReleaseLib("dvReleaseLib", "true"),
#elif (defined PH_DEBUG_WITH_RELEASE_LIBS)
	m_varDebugWithDebugLib("dvDebugWithDebugLib", "false"),
	m_varDebugWithReleaseLib("dvDebugWithReleaseLib", "true"),
	m_varReleaseLib("dvReleaseLib", "false"),
#elif (defined PH_DEBUG_WITH_DEBUG_LIBS)
	m_varDebugWithDebugLib("dvDebugWithDebugLib", "true"),
	m_varDebugWithReleaseLib("dvDebugWithReleaseLib", "false"),
	m_varReleaseLib("dvReleaseLib", "false"),
#endif
#ifdef PH_DEBUG
	m_varDebug("dvDebug", "true"),
	m_varRelease("dvRelease", "false"),
#else
	m_varDebug("dvDebug", "false"),
	m_varRelease("dvRelease", "true"),
#endif
#ifdef PH_LINUX
	m_varLinux("dvLinux", "true"),
	#ifdef PH_DEBUG
		m_varLinuxDebug("dvLinuxDebug", "true"),
		m_varLinuxRelease("dvLinuxRelease", "false"),
	#else
		m_varLinuxDebug("dvLinuxDebug", "false"),
		m_varLinuxRelease("dvLinuxRelease", "true"),
	#endif
	m_varWindows("dvWindows", "false"),
	m_varWindowsDebug("dvWindowsDebug", "false"),
	m_varWindowsRelease("dvWindowsRelease", "false"),
#else
	m_varLinux("dvLinux", "false"),
	m_varLinuxDebug("dvLinuxDebug", "false"),
	m_varLinuxRelease("dvLinuxRelease", "false"),
	m_varWindows("dvWindows", "true"),
	#ifdef PH_DEBUG
		m_varWindowsDebug("dvWindowsDebug", "true"),
		m_varWindowsRelease("dvWindowsRelease", "false"),
	#else
		m_varWindowsDebug("dvWindowsDebug", "false"),
		m_varWindowsRelease("dvWindowsRelease", "true"),
	#endif		
#endif
#ifdef PH_SDL
	m_varSDL("dvSDL", "true"),
	#ifdef PH_DEBUG
		m_varSDLDebug("dvSDLDebug", "true"),
		m_varSDLRelease("dvSDLRelease", "false")
	#else
		m_varSDLDebug("dvSDLDebug", "false"),
		m_varSDLRelease("dvSDLRelease", "true")
	#endif	
#else 
	m_varSDL("dvSDL", "false"),
	m_varSDLDebug("dvSDLDebug", "false"),
	m_varSDLRelease("dvSDLRelease", "false")
#endif	
{
	m_cmdEcho.SetProc(PH_CONTEXT_CMD_BIND(&Context::CmdEcho, this));
	m_cmdHelp.SetProc(PH_CONTEXT_CMD_BIND(&Context::CmdListCmds, this));
	m_cmdListCmds.SetProc(PH_CONTEXT_CMD_BIND(&Context::CmdListCmds, this));
	m_cmdListVars.SetProc(PH_CONTEXT_CMD_BIND(&Context::CmdListVars, this));
	m_cmdSet.SetProc(PH_CONTEXT_CMD_BIND(&Context::CmdSet, this));
	m_cmdIf.SetProc(PH_CONTEXT_CMD_BIND(&Context::CmdIf, this));

	this->AddContextCommand(m_cmdEcho);
	this->AddContextCommand(m_cmdHelp);
	this->AddContextCommand(m_cmdListCmds);
	this->AddContextCommand(m_cmdListVars);
	this->AddContextCommand(m_cmdSet);
	this->AddContextCommand(m_cmdIf);

	this->AddContextVariable(m_varDebugWithDebugLib);
	this->AddContextVariable(m_varDebugWithReleaseLib);
	this->AddContextVariable(m_varReleaseLib);
	this->AddContextVariable(m_varDebug);
	this->AddContextVariable(m_varRelease);
	this->AddContextVariable(m_varLinux);
	this->AddContextVariable(m_varLinuxDebug);
	this->AddContextVariable(m_varWindows);
	this->AddContextVariable(m_varWindowsDebug);
	this->AddContextVariable(m_varWindowsRelease);
	this->AddContextVariable(m_varSDL);
	this->AddContextVariable(m_varSDLDebug);
}

void Phobos::Shell::Context::AddContextVariable(Variable &var)
{
	AddItem(m_setVariables, var, "Variable", "Context_c::AddContextVar");
}

void Phobos::Shell::Context::RemoveContextVariable(Variable &var)
{
	var.unlink();
}

void Phobos::Shell::Context::SetContextVariableValue(const String_t &name, const String_t &value)
{
	auto &var(const_cast<Variable&>(this->GetContextVariable(name)));

	var.SetValue(value);
}

const Phobos::Shell::Variable &Phobos::Shell::Context::GetContextVariable(const String_t &name) const
{
	const auto *p = this->TryGetContextVariable(name);
	if(p == NULL)
	{
		std::stringstream str;
		str << "Variable " << name << " not found.";
		PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "&Context_c::GetContextVar", str.str());
	}

	return *p;
}

const Phobos::Shell::Variable *Phobos::Shell::Context::TryGetContextVariable(const String_t &name) const
{
	return TryGetItem(m_setVariables, name);
}

Phobos::Shell::Variable *Phobos::Shell::Context::TryGetContextVariable(const String_t &name)
{
	return const_cast<Phobos::Shell::Variable *>(static_cast<const Context *>(this)->TryGetContextVariable(name));
}

const Phobos::Shell::Command *Phobos::Shell::Context::TryGetContextCommand(const String_t &name) const
{
	return TryGetItem(m_setCommands, name);
}

Phobos::Shell::Command *Phobos::Shell::Context::TryGetContextCommand(const String_t &name)
{
	return const_cast<Phobos::Shell::Command *>(static_cast<const Context *>(this)->TryGetContextCommand(name));
}

void Phobos::Shell::Context::AddContextCommand(Command &cmd)
{
	AddItem(m_setCommands, cmd, "Command", "Context_c::AddContextCmd");
}

void Phobos::Shell::Context::RemoveContextCommand(Command &cmd)
{
	cmd.unlink();
}

bool Phobos::Shell::Context::GetCmdLine(String_t &dest, const String_t &src, size_t &currentPos)
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

void Phobos::Shell::Context::ParseCmdParam(String_t &result, const String_t &param)
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
			auto *var = this->TryGetContextVariable(result);
			if(var == NULL)
			{
				Phobos::LogMakeStream() << "[Context_c::ParseCmdParam] Error: cant find variable " << result;
				return;
			}
			else
				result = var->GetValue();
		}
	}
	else
		result = param;
}

bool Phobos::Shell::Context::ParseCmdLine(StringVector_t &args, const String_t &cmdLine)
{
	size_t pos;
	size_t start;
	size_t dataSize;
	Char_t ch;

	String_t tempStr;
	String_t param;

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

void Phobos::Shell::Context::ExecuteCmdLine(StringVector_t &args)
{
	PH_ASSERT(!args.empty());

	const String_t	&cmdName = args[0];

	auto *cmd = this->TryGetContextCommand(cmdName);
	if(cmd == NULL)
	{
		LogMakeStream() << "[Context_c::ExecuteCmdLine] Command " << cmdName << " not found";
	}
	else
		cmd->Execute(args, *this);
}

/**

	Called to execute the buffer. This methods parses cmdBuffer and executes any commands
	that it may find. The cmdBuffer can be composed of several cmdLines.

	To get errors from commands, they should push their errors on stack (IM_ErrorManager_c).

	\param cmdBuffer string with commands to be executed.
	\param contextParam parameter that is stored on IM_ContextStringContainer_c.

*/
void Phobos::Shell::Context::Execute(const String_t &cmdBuffer)
{
	size_t			currentPos = 0;
	String_t		cmdLine;
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

void Phobos::Shell::Context::ExecuteFromFile(const String_t &fileName)
{
	std::ifstream input(fileName.c_str(), ios_base::in);		

	if(input.fail())
		PH_RAISE(FILE_NOT_FOUND_EXCEPTION, "Context_c::ExecuteFromFile", "'" + fileName + "' not found");

	String_t line;
	while(input.good())
	{
		getline(input, line);
		this->Execute(line);
	}
}


// =====================================================
// COMMANDS
// =====================================================

void Phobos::Shell::Context::CmdEcho(const StringVector_t &args, Context &)
{
	size_t sz = args.size();

	stringstream stream;
	if(1 == sz)
	{
		stream << "[DCmdEcho] Error: insuficient parameters" << endl;
		stream << "\tUsage: echo <param1> [<paramn>]" << endl;
		stream << "\tExample: echo hallo $varName\n";
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
	LogMessage(stream.str());
}

void Phobos::Shell::Context::CmdListCmds(const StringVector_t &args, Context &)
{
	std::stringstream stream;

	for(auto &cmd: m_setCommands)
	{
		stream << "\t" << cmd.GetName() << std::endl;
	}

	LogMessage(stream.str());
}

void Phobos::Shell::Context::CmdListVars(const StringVector_t &args, Context &)
{
	std::stringstream stream;

	for(auto &var : m_setVariables)		
	{
		stream << "\t" << var.GetName() << " = " << var.GetValue() << std::endl;
	}

	LogMessage(stream.str());
}

void Phobos::Shell::Context::CmdSet(const StringVector_t &args, Context &)
{
	stringstream stream;
	if(args.size() < 3)
	{
		stream	<< "[CmdSet] Error: insuficient parameters" << endl
				<< "[CmdSet] Usage: set <varName> <varValue>" << endl
				<< "\tExample: set MyPath \"blabla\"";
	}
	else
	{
		auto *var = this->TryGetContextVariable(args[1]);
		if(var)
		{
			try
			{
				var->SetValue(args[2]);

				return;
			}
			catch(Exception &e)
			{
				stream << "[CmdSet] Error: cant set variable " << var->GetName() << " value to " << args[2] << ", message: " << e.what();
			}
		}
		else
		{
			stream << "[CmdSet] Error: variable " << args[1] << " not found";
		}
	}

	LogMessage(stream.str());
}


void Phobos::Shell::Context::CmdIf(const StringVector_t &args, Context &)
{
	size_t sz = args.size();

	if(sz < 3)
	{
		LogMessage("[CmdIf] Insuficient parameters, usage: if <cond> <expr> [expr2]");
	}
	else
	{
		const String_t &expr = args[1];
		const String_t *result = NULL;

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


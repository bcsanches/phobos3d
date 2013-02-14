/*
Phobos 3d
May 2012
Copyright (c) 2005-2013 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "Phobos/Engine/Console.h"

#include <Phobos/Exception.h>
#include <Phobos/ObjectManager.h>
#include <Phobos/Path.h>

#include <Phobos/Register/Table.h>
#include <Phobos/Register/Manager.h>

#include <Phobos/Shell/Utils.h>

#include <Phobos/System/InputActions.h>
#include <Phobos/System/InputEvent.h>

#define CONSOLE_HISTORY_COUNT 32

#define CONSOLE_LINE_COUNT 15


Phobos::Engine::ConsolePtr_t Phobos::Engine::Console::ipInstance_gl;
	
Phobos::Engine::Console &Phobos::Engine::Console::GetInstance(void)
{
	return *ipInstance_gl;
}

void Phobos::Engine::Console::ReleaseInstance(void)
{
	ipInstance_gl.reset();
}
			
Phobos::Engine::Console::Console(const String_t &name, UInt32_t flags):
	Module(name, flags),
	m_cmdLs("ls"),
	m_cmdCd("cd"),
	m_cmdDumpTable("dumpTable"),		
	m_lstText(CONSOLE_LINE_COUNT),
	m_lstHistory(CONSOLE_HISTORY_COUNT),
	m_strCurrentNodePathName("/"),
	m_fActive(true)
{
	LogAddListener(*this);				

	m_cmdLs.SetProc(PH_CONTEXT_CMD_BIND(&Console::CmdLs, this));
	m_cmdCd.SetProc(PH_CONTEXT_CMD_BIND(&Console::CmdCd, this));
	m_cmdDumpTable.SetProc(PH_CONTEXT_CMD_BIND(&Console::CmdDumpTable, this));		

	m_clContext.AddContextCommand(m_cmdLs);
	m_clContext.AddContextCommand(m_cmdCd);
	m_clContext.AddContextCommand(m_cmdDumpTable);
}

Phobos::Engine::Console::~Console()
{
	//empty
}

void Phobos::Engine::Console::UpdateInstance(ConsolePtr_t console)
{
	ipInstance_gl = console;
}

const Phobos::String_t &Phobos::Engine::Console::EditBoxStr() const
{
	return m_clEditBox.GetStr();
}

void Phobos::Engine::Console::Execute(const String_t &cmdLine)
{
	this->QueueCommand(cmdLine);
}

void Phobos::Engine::Console::ExecuteFromFile(const String_t &fileName)
{
	m_clContext.ExecuteFromFile(fileName);
}

void Phobos::Engine::Console::OnChar(Char_t ch)
{		
	switch(ch)
	{
		case System::KB_BACKSPACE:
			m_clEditBox.Backspace();
			break;

		case System::KB_ENTER:
			this->OnEnter();
			break;

		case System::KB_ESCAPE:
			//ignore
			break;

		default:
			m_clEditBox.AddChar(ch);
			break;
	}

	this->OnEditBoxChanged();
}

void Phobos::Engine::Console::OnEnter(void)
{
	const String_t &cmdLine = m_clEditBox.GetStr();

	LogMakeStream() << "> " << cmdLine;
		
	this->QueueCommand(cmdLine);		

	this->AddToHistory(cmdLine);

	m_clEditBox.Clear();
}

void Phobos::Engine::Console::QueueCommand(const String_t &cmd)
{
	m_clCommandBuffer << cmd << std::endl;
}

void Phobos::Engine::Console::FlushCommandBuffer()
{
	String_t buffer = m_clCommandBuffer.str();
	m_clCommandBuffer.str(String_t());
	m_clCommandBuffer.clear();

	m_clContext.Execute(buffer);				
}

void Phobos::Engine::Console::AddToHistory(const String_t &str)
{
	if(!m_lstHistory.empty() && m_lstHistory.front().compare(str) == 0)
		return;

	m_lstHistory.push_front(str);
	m_itPrevCmd = m_lstHistory.begin();
}

bool Phobos::Engine::Console::GetPreviousCommand(String_t &str)
{
	if(!m_lstHistory.empty() && (m_itPrevCmd != m_lstHistory.end()))
	{
		str = *m_itPrevCmd;
		++m_itPrevCmd;

		return(true);
	}

	return(false);
}

bool Phobos::Engine::Console::GetNextCommand(String_t &str)
{
	if(!m_lstHistory.empty() && (m_itPrevCmd != m_lstHistory.begin()))
	{
		--m_itPrevCmd;
		str = *m_itPrevCmd;

		return(true);
	}

	return(false);
}

void Phobos::Engine::Console::OnPreviousCommand(void)
{
	String_t tmp;

	if(this->GetPreviousCommand(tmp))
	{			
		m_clEditBox.SetStr(tmp);

		this->OnEditBoxChanged();
	}
}

void Phobos::Engine::Console::OnNextCommand(void)
{
	String_t tmp;

	if(this->GetNextCommand(tmp))
	{			
		m_clEditBox.SetStr(tmp);

		this->OnEditBoxChanged();
	}
}	

void Phobos::Engine::Console::Print(const String_t &str)
{
	String_t line;
	size_t splitPos = 0;

	while(StringSplitBy(line, str, '\n', splitPos, &splitPos))
	{
		m_lstText.push_back(line);
	}

	this->OnTextListChanged();		
}

bool Phobos::Engine::Console::HandleInputEvent(const System::InputEvent_s &event)
{
	switch(event.m_eType)
	{
		case System::INPUT_EVENT_CHAR:
			if(this->IsActive())
			{
				this->OnChar(static_cast<Char_t>(event.m_stChar.m_u16Char));
				return true;
			}
			break;

		case System::INPUT_EVENT_BUTTON:
			if(event.m_stButton.m_eState == System::BUTTON_STATE_DOWN)
			{
				if(this->IsActive())
				{
					if(event.m_stButton.m_uId == System::KB_UP_ARROW)
					{
						this->OnPreviousCommand();
						return true;
					}
					else if(event.m_stButton.m_uId == System::KB_DOWN_ARROW)
					{
						this->OnNextCommand();
						return true;
					}
				}
			}
			break;

        default:
            break;
	}

	return false;
}

void Phobos::Engine::Console::OnLogMessage(const String_t &msg)
{
	this->Print(msg);
}

void Phobos::Engine::Console::ToggleConsole(void)
{
	const Char_t *msg;

	//if just turning on
	m_fActive = !m_fActive;
	if(m_fActive)
	{			
		msg = "enabled";					
	}
	else
	{
		msg = "disabled";
	}

	std::string tmp("Console ");
	tmp.append(msg);
	LogMessage(tmp);

	this->OnToggleConsole();
}	

void Phobos::Engine::Console::AddContextVariable(Shell::Variable &var)
{
	return m_clContext.AddContextVariable(var);
}

void Phobos::Engine::Console::AddContextCommand(Shell::Command &cmd)
{
	return m_clContext.AddContextCommand(cmd);
}	

void Phobos::Engine::Console::RemoveContextCommand(Shell::Command &cmd)
{
	return m_clContext.RemoveContextCommand(cmd);
}

const Phobos::Shell::Variable &Phobos::Engine::Console::GetContextVariable(const String_t &name) const
{
	return m_clContext.GetContextVariable(name);
}

/*
Shell::Variable *Console::TryGetContextVariable(const String_t &name)
{
	return m_clContext.TryGetContextVariable(name);
}*/

const Phobos::Shell::Variable *Phobos::Engine::Console::TryGetContextVariable(const String_t &name) const
{
	return m_clContext.TryGetContextVariable(name);
}

void Phobos::Engine::Console::CmdLs(const Shell::StringVector_t &args, Shell::Context &)
{		
	Node		*currentNode;

	if(args.size() > 1)
	{
		Path path(args[1]);

		if(path.IsRelative())
		{
			path = m_strCurrentNodePathName;
			path.AddName(args[1]);
		}

		try
		{
			currentNode = &ObjectManager::LookupObject(path);
		}
		catch(ObjectNotFoundException &)
		{
			LogMakeStream() << "[OgreConsole::CmdLs] Invalid path: " << path.GetStr();

			return;
		}
	}
	else
	{
		try
		{
			currentNode = &ObjectManager::LookupObject(Path(m_strCurrentNodePathName));
		}
		catch(ObjectNotFoundException &)
		{
			LogMakeStream() << "[OgreConsole::CmdLs] Invalid node (" << m_strCurrentNodePathName << "), going to root";

			m_strCurrentNodePathName = "/";
			return;
		}
	}

	std::stringstream stream;

	stream << "\t.\n\t..\n";
	for(const auto &pair : currentNode->GetNodes())		
	{
		stream << "\t" << pair.second->GetName() << "\n";
	}

	LogMessage(stream.str());
}

void Phobos::Engine::Console::CmdCd(const Shell::StringVector_t &args, Shell::Context &)
{
	if(args.size() == 2)
	{			
		Node	*currentNode;
		Path	path(args[1]);

		if(path.IsRelative())
		{
			path = m_strCurrentNodePathName;
			path.AddName(args[1]);
		}

		try
		{
			currentNode = &ObjectManager::LookupObject(path);
			currentNode->GetThisPath(path);

			//if we are at root, we set the path manually, otherwise the path will be empty
			if(currentNode->GetParent() == NULL)
				m_strCurrentNodePathName = '/';
			else
				m_strCurrentNodePathName = path.GetStr();
		}
		catch(const Exception &)
		{
			LogMakeStream() << "[OgreConsole::CmdCd] Invalid dir " << args[1];
		}
	}
	else
		LogMessage("[OgreConsole::CmdCd] Insuficient parameters");
}

void Phobos::Engine::Console::CmdDumpTable(const Shell::StringVector_t &args, Shell::Context &)
{
	auto stream = LogMakeStream();

	try
	{
		Register::Table *table;
		if(args.size() == 2)
		{
			table = &Register::GetTable(Path(args[1]));
			stream << "Values on " << args[1] << '\n';
		}
		else if(args.size() == 3)
		{
			table = &Register::GetTable(args[1], args[2]);
			stream << "Values on " << args[2];
		}
		else
		{
			stream << "[OgreConsole::CmdDumpTable] Insuficient parameters, usage: dumpTable <path>\n";
			return;
		}

		stream << "FIXME\n";
		/*
		for(Dictionary_c::StringMapConstIterator_t it = dict->begin(), end = dict->end(); it != end; ++it)
		{
			stream << it->first << " = " << it->second << "\n";
		}
		*/
	}
	catch(std::exception &e)
	{
		stream << e.what();
	}
}

//
//
//EDIT BOX
//
//
inline Phobos::Engine::Console::EditBox::EditBox(void):
	m_uCursorPos(0)
{

}

inline void Phobos::Engine::Console::EditBox::AddChar(Char_t ch)
{
	m_strStr.insert(m_uCursorPos, 1, ch);

	++m_uCursorPos;
}


inline void Phobos::Engine::Console::EditBox::RetreatCursor(void)
{
	if(m_uCursorPos > 0)
		--m_uCursorPos;
}

inline void Phobos::Engine::Console::EditBox::AdvanceCursor(void)
{
	if(m_uCursorPos <= m_strStr.length())
		++m_uCursorPos;
}

inline void Phobos::Engine::Console::EditBox::Backspace(void)
{
	if(m_uCursorPos > 0)
	{
		--m_uCursorPos;
		m_strStr.erase(m_uCursorPos, 1);
	}
}

inline void Phobos::Engine::Console::EditBox::Clear(void)
{
	m_strStr.clear();
	m_uCursorPos = 0;
}

inline void Phobos::Engine::Console::EditBox::SetStr(const String_t &str)
{
	m_strStr = str;

	m_uCursorPos = m_strStr.length();
}

inline const Phobos::String_t &Phobos::Engine::Console::EditBox::GetStr(void) const
{
	return(m_strStr);
}


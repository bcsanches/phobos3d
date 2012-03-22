/*
Phobos 3d
May 2012
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

#include "PH_Console.h"

#include <boost/foreach.hpp>

#include <PH_ContextUtils.h>
#include <PH_Dictionary.h>
#include <PH_DictionaryManager.h>
#include <PH_Exception.h>
#include <PH_InputActions.h>
#include <PH_InputEvent.h>
#include <PH_InputManager.h>
#include <PH_Kernel.h>
#include <PH_Path.h>

#define CONSOLE_KEY '`'

#define CONSOLE_HISTORY_COUNT 32

#define CONSOLE_LINE_COUNT 15

namespace Phobos
{
	ConsolePtr_t Console_c::ipInstance_gl;
	
	ConsolePtr_t Console_c::GetInstance(void)
	{
		return ipInstance_gl;
	}

	void Console_c::ReleaseInstance(void)
	{
		ipInstance_gl.reset();
	}
			
	Console_c::Console_c(const String_c &name, ChildrenMode_e childrenMode):
		CoreModule_c(name, childrenMode),
		cmdLs("ls"),
		cmdCd("cd"),
		cmdDumpTable("dumpTable"),
		cmdToggleConsole("toggleConsole"),
		lstText(CONSOLE_LINE_COUNT),
		lstHistory(CONSOLE_HISTORY_COUNT),
		strCurrentNodePathName("/"),
		fIgnoreFirstChar(false),
		fIgnoredLastChar(false),		
		fActive(true)
	{
		InputManager_c::CreateInstance("InputManager")->AddListener(*this);

		Kernel_c::GetInstance().AddLogListener(*this);

		ipInputMapper = InputMapper_c::Create("InputMapper", clContext);
		ipInputMapper->Disable();

		this->AddChild(ipInputMapper);

		cmdLs.SetProc(PH_CONTEXT_CMD_BIND(&Console_c::CmdLs, this));
		cmdCd.SetProc(PH_CONTEXT_CMD_BIND(&Console_c::CmdCd, this));
		cmdDumpTable.SetProc(PH_CONTEXT_CMD_BIND(&Console_c::CmdDumpTable, this));
		cmdToggleConsole.SetProc(PH_CONTEXT_CMD_BIND(&Console_c::CmdToggleConsole, this));

		clContext.AddContextCmd(cmdLs);
		clContext.AddContextCmd(cmdCd);
		clContext.AddContextCmd(cmdDumpTable);
		clContext.AddContextCmd(cmdToggleConsole);
	}

	Console_c::~Console_c()
	{
		InputManager_c::ReleaseInstance();
	}

	void Console_c::UpdateInstance(ConsolePtr_t console)
	{
		ipInstance_gl = console;
	}

	const String_c &Console_c::EditBoxStr() const
	{
		return clEditBox.GetStr();
	}

	void Console_c::Execute(const String_c &cmdLine)
	{
		clContext.Execute(cmdLine);
	}

	void Console_c::ExecuteFromFile(const String_c &fileName)
	{
		clContext.ExecuteFromFile(fileName);
	}

	void Console_c::OnChar(Char_t ch)
	{
		if(fIgnoreFirstChar)
		{
			fIgnoreFirstChar = false;
			fIgnoredLastChar = true;
			return;
		}

		if((fIgnoredLastChar) && (ch == CONSOLE_KEY))
		{
			fIgnoredLastChar = false;
			return;
		}

		fIgnoredLastChar = false;

		switch(ch)
		{
			case KB_BACKSPACE:
				clEditBox.Backspace();
				break;

			case KB_ENTER:
				this->OnEnter();
				break;

			default:
				clEditBox.AddChar(ch);
				break;
		}

		this->OnEditBoxChanged();
	}

	void Console_c::OnEnter(void)
	{
		const String_c &cmdLine = clEditBox.GetStr();

		Kernel_c::GetInstance().LogStream() << "> " << cmdLine;

		clContext.Execute(cmdLine);

		this->AddToHistory(cmdLine);

		clEditBox.Clear();
	}

	void Console_c::AddToHistory(const String_c &str)
	{
		if(!lstHistory.empty() && lstHistory.front().compare(str) == 0)
			return;

		lstHistory.push_front(str);
		itPrevCmd = lstHistory.begin();
	}

	bool Console_c::GetPreviousCommand(String_c &str)
	{
		if(!lstHistory.empty() && (itPrevCmd != lstHistory.end()))
		{
			str = *itPrevCmd;
			++itPrevCmd;

			return(true);
		}

		return(false);
	}

	bool Console_c::GetNextCommand(String_c &str)
	{
		if(!lstHistory.empty() && (itPrevCmd != lstHistory.begin()))
		{
			--itPrevCmd;
			str = *itPrevCmd;

			return(true);
		}

		return(false);
	}

	void Console_c::OnPreviousCommand(void)
	{
		String_c tmp;

		if(this->GetPreviousCommand(tmp))
		{			
			clEditBox.SetStr(tmp);

			this->OnEditBoxChanged();
		}
	}

	void Console_c::OnNextCommand(void)
	{
		String_c tmp;

		if(this->GetNextCommand(tmp))
		{			
			clEditBox.SetStr(tmp);

			this->OnEditBoxChanged();
		}
	}	

	void Console_c::Print(const String_c &str)
	{
		String_c line;
		size_t splitPos = 0;

		while(StringSplitBy(line, str, '\n', splitPos, &splitPos))
		{
			lstText.push_back(line);
		}

		this->OnTextListChanged();		
	}

	void Console_c::OnFixedUpdate()
	{
		InputManager_c::GetInstance()->Update();
	}

	void Console_c::InputManagerEvent(const InputManagerEvent_s &event)
	{
		std::stringstream stream;

		switch(event.eType)
		{
			case INPUT_MANAGER_EVENT_DEVICE_ATTACHED:
				stream << "[Console_c::InputManagerEvent] Device " << event.ipDevice->GetName() << " attached.";
				if(event.ipDevice->GetDeviceType() == INPUT_DEVICE_KEYBOARD)
					event.ipDevice->AddListener(*this);
				break;

			case INPUT_MANAGER_EVENT_DEVICE_DETACHED:
				stream << "[Console_c::InputManagerEvent] Device " << event.ipDevice->GetName() << " detached.";
				break;
		}

		Kernel_c::GetInstance().LogMessage(stream.str());
	}

	void Console_c::InputEvent(const InputEvent_s &event)
	{
		switch(event.eType)
		{
			case INPUT_EVENT_CHAR:
				if(this->IsActive())
					this->OnChar(static_cast<Char_t>(event.stChar.u16Char));
				break;

			case INPUT_EVENT_BUTTON:
				if(event.stButton.eState == BUTTON_STATE_DOWN)
				{
					if(event.stButton.uId == CONSOLE_KEY || event.stButton.uId == '\'')
						this->ToggleConsole();
					else if(this->IsActive())
					{
						if(event.stButton.uId == KB_UP_ARROW)
							this->OnPreviousCommand();
						else if(event.stButton.uId == KB_DOWN_ARROW)
							this->OnNextCommand();
					}
				}
				break;

            default:
                break;
		}
	}

	void Console_c::Message(const String_c &msg)
	{
		this->Print(msg);
	}

	void Console_c::ToggleConsole(void)
	{
		const Char_t *msg;

		//if just turning on, ignore first input
		fActive = !fActive;
		if(fActive)
		{
			fIgnoreFirstChar = true;
			msg = "enabled";

			ipInputMapper->Disable();			
		}
		else
		{
			msg = "disabled";

			ipInputMapper->Enable();
		}

		std::string tmp("Console ");
		tmp.append(msg);
		Kernel_c::GetInstance().LogMessage(tmp);

		this->OnToggleConsole();
	}	

	void Console_c::CmdLs(const StringVector_t &args, Context_c &)
	{
		Kernel_c	&kernel = Kernel_c::GetInstance();
		NodePtr_t	currentNode;

		if(args.size() > 1)
		{
			Path_c path(args[1]);

			if(path.IsRelative())
			{
				path = strCurrentNodePathName;
				path.AddName(args[1]);
			}

			try
			{
				currentNode = kernel.LookupObject(path);
			}
			catch(ObjectNotFoundException_c &)
			{
				kernel.LogStream() << "[OgreConsole_c::CmdLs] Invalid path: " << path.GetStr();

				return;
			}
		}
		else
		{
			try
			{
				currentNode = kernel.LookupObject(Path_c(strCurrentNodePathName));
			}
			catch(ObjectNotFoundException_c &)
			{
				kernel.LogStream() << "[OgreConsole_c::CmdLs] Invalid node (" << strCurrentNodePathName << "), going to root";

				strCurrentNodePathName = "/";
				return;
			}
		}

		std::stringstream stream;

		stream << "\t.\n\t..\n";
		BOOST_FOREACH(const Node_c::NodeMapPair_t &pair, currentNode->GetNodes())
		{
			stream << "\t" << pair.second->GetName() << "\n";
		}

		kernel.LogMessage(stream.str());
	}

	void Console_c::CmdCd(const StringVector_t &args, Context_c &)
	{
		if(args.size() == 2)
		{
			Kernel_c	&kernel = Kernel_c::GetInstance();
			NodePtr_t	currentNode;
			Path_c		path(args[1]);

			if(path.IsRelative())
			{
				path = strCurrentNodePathName;
				path.AddName(args[1]);
			}

			try
			{
				currentNode = kernel.LookupObject(path);
				currentNode->GetThisPath(path);

				//if we are at root, we set the path manually, otherwise the path will be empty
				if(currentNode->GetParent() == NULL)
					strCurrentNodePathName = '/';
				else
					strCurrentNodePathName = path.GetStr();
			}
			catch(const Exception_c &)
			{
				kernel.LogStream() << "[OgreConsole_c::CmdCd] Invalid dir " << args[1];
			}
		}
		else
			Kernel_c::GetInstance().LogMessage("[OgreConsole_c::CmdCd] Insuficient parameters");
	}

	void Console_c::CmdDumpTable(const StringVector_t &args, Context_c &)
	{
		Log_c::Stream_c stream = Kernel_c::GetInstance().LogStream();

		try
		{
			DictionaryPtr_t dict;
			if(args.size() == 2)
			{
				dict = DictionaryManager_c::GetInstance()->GetDictionary(Path_c(args[1]));
				stream << "Values on " << args[1] << '\n';
			}
			else if(args.size() == 3)
			{
				dict = DictionaryManager_c::GetInstance()->GetDictionary(args[1], args[2]);
				stream << "Values on " << args[2];
			}
			else
			{
				stream << "[OgreConsole_c::CmdDumpTable] Insuficient parameters, usage: dumpTable <path>\n";
				return;
			}

			for(Dictionary_c::StringMapConstIterator_t it = dict->begin(), end = dict->end(); it != end; ++it)
			{
				stream << it->first << " = " << it->second << "\n";
			}
		}
		catch(std::exception &e)
		{
			stream << e.what();
		}
	}

	void Console_c::CmdToggleConsole(const StringVector_t &args, Context_c &)
	{
		this->ToggleConsole();
	}

	//
	//
	//EDIT BOX
	//
	//
	inline Console_c::EditBox_c::EditBox_c(void):
		uCursorPos(0)
	{

	}

	inline void Console_c::EditBox_c::AddChar(Char_t ch)
	{
		strStr.insert(uCursorPos, 1, ch);

		++uCursorPos;
	}


	inline void Console_c::EditBox_c::RetreatCursor(void)
	{
		if(uCursorPos > 0)
			--uCursorPos;
	}

	inline void Console_c::EditBox_c::AdvanceCursor(void)
	{
		if(uCursorPos <= strStr.length())
			++uCursorPos;
	}

	inline void Console_c::EditBox_c::Backspace(void)
	{
		if(uCursorPos > 0)
		{
			--uCursorPos;
			strStr.erase(uCursorPos, 1);
		}
	}

	inline void Console_c::EditBox_c::Clear(void)
	{
		strStr.clear();
		uCursorPos = 0;
	}

	inline void Console_c::EditBox_c::SetStr(const String_c &str)
	{
		strStr = str;

		uCursorPos = strStr.length();
	}

	inline const String_c &Console_c::EditBox_c::GetStr(void) const
	{
		return(strStr);
	}
}

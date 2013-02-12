/*
Phobos 3d
January 2010
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

#include "Phobos/Log.h"

#include <ctime>
#include <iostream>

namespace Phobos
{
	Log::Log(const String_t &name, UInt_t flags):
		m_fCopyToStdout(flags & LOG_FLAG_COPY_TO_STDOUT)
	{
		m_clFile.open(name.c_str());

		time_t t;		
		time(&t);		

		this->MakeStream() << "Log created at " << ctime(&t);
	}

	Log::~Log()
	{		
		//empty
	}

	void Log::Message(const String_t &message)
	{			
		if(m_fCopyToStdout)
			std::cout << message << std::endl;

		m_clFile << message << std::endl;
		m_clFile.flush();

		for(auto &listener : m_lstListeners)
			listener.OnLogMessage(message);
	}

	void Log::AddListener(Listener &listener)
	{
		m_lstListeners.push_back(listener);
	}

	void Log::RemoveListener(Listener &listener)
	{
		listener.m_hkListener.unlink();
	}

	size_t Log::GetListenersCount() const
	{
		return m_lstListeners.size();
	}

	void Log::swap(Log &rhs)
	{
		std::swap(m_clFile, rhs.m_clFile);
		std::swap(m_lstListeners, rhs.m_lstListeners);
		std::swap(m_fCopyToStdout, rhs.m_fCopyToStdout);
	}
}

//
//
//GLOBAL LOG
//
//

namespace 
{
	static Phobos::Log &GetLog()
	{
		static Phobos::Log log("Phobos.log", Phobos::LOG_FLAG_COPY_TO_STDOUT);

		return log;
	}
}

void Phobos::LogMessage(const String_t &message)
{
	GetLog().Message(message);
}

Phobos::Log::Stream Phobos::LogMakeStream()
{
	return GetLog().MakeStream();
}

void Phobos::LogAddListener(Log::Listener &listener)
{
	GetLog().AddListener(listener);
}

void Phobos::LogRemoveListener(Log::Listener &listener)
{
	GetLog().RemoveListener(listener);
}

PH_BASE_API void Phobos::LogChangeFile(const char *newFile)
{
	Log log(newFile, LOG_FLAG_COPY_TO_STDOUT);

	GetLog().swap(log);
}

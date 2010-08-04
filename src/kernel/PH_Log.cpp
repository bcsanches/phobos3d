/*
Phobos 3d
  version 0.0.1, January 2010

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

#include "PH_Log.h"

#include <ctime>
#include <iostream>

#include <boost/bind.hpp>

namespace Phobos
{
	Log_c::Log_c(const String_c &name, UInt_t flags):
		fCopyToStdout(flags & LOG_FLAG_COPY_TO_STDOUT)
	{
		clFile.open(name.c_str());

		time_t t;		
		time(&t);

		std::stringstream stream;
		stream << "Log created at " << ctime(&t);

		this->Message(stream.str());
	}

	Log_c::~Log_c()
	{
		//empty
	}

	void Log_c::Message(const String_c &message)
	{				
		std::for_each(lstListeners.begin(), lstListeners.end(), boost::bind(&LogListener_c::Message, _1, message));		

		if(fCopyToStdout)
			std::cout << message << std::endl;

		clFile << message << std::endl;
		clFile.flush();
	}

	void Log_c::AddListener(LogListener_c &listener)
	{
		lstListeners.push_back(listener);
	}

	void Log_c::RemoveListener(LogListener_c &listener)
	{
		listener.hkListener.unlink();
	}

	size_t Log_c::GetListenersCount() const
	{
		return lstListeners.size();
	}
}

/*
Phobos 3d
  January 2010

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

#ifndef PH_LOG_H
#define PH_LOG_H

#include <fstream>

#include <boost/intrusive/list.hpp>
#include <boost/utility.hpp>

#include "PH_Defs.h"
#include "PH_Listener.h"
#include "PH_KernelAPI.h"
#include "PH_String.h"

namespace Phobos
{
	class PH_KERNEL_API LogListener_c
	{
		public:
			virtual ~LogListener_c() {};

			virtual void Message(const String_c &message) = 0;
			
			PH_DECLARE_LISTENER_HOOK
	};

	class PH_KERNEL_API Log_c: boost::noncopyable
	{
		public:
			Log_c(const String_c &name);
			~Log_c();

			void Message(const String_c &message);

			void AddListener(LogListener_c &listener);
			void RemoveListener(LogListener_c &listener);
			size_t GetListenersCount() const;

		private:
			PH_DECLARE_LISTENER_LIST_TYPE(LogListener_c);

			std::ofstream		clFile;
			ListenersList_t		lstListeners;
	};
}

#endif

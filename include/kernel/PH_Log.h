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
#ifndef PH_LOG_H
#define PH_LOG_H

#include <iostream>
#include <fstream>

#include <boost/intrusive/list.hpp>

#include "PH_Defs.h"
#include "PH_DisableCopy.h"
#include "PH_Listener.h"
#include "PH_KernelAPI.h"
#include "PH_String.h"

namespace Phobos
{
	enum LogFlags_e
	{
		LOG_FLAG_COPY_TO_STDOUT = 0x01
	};

	class PH_KERNEL_API LogListener_c
	{
		public:
			virtual ~LogListener_c() {};

			virtual void Message(const String_c &message) = 0;
			
			PH_DECLARE_LISTENER_HOOK
	};

	class PH_KERNEL_API Log_c
	{
		PH_DISABLE_COPY(Log_c);

		public:
			class Stream_c
			{
				protected:
					Log_c &rclTarget;					
					std::stringstream strsStream;					
		
				public:
					/// Simple type to indicate a flush of the stream to the log
					struct Flush {};
					struct Endl {};

					Stream_c(Log_c &target):
						rclTarget(target)
					{
						//empty
					}

					// copy constructor
					Stream_c(const Stream_c& rhs): 
						rclTarget(rhs.rclTarget)
					{
						strsStream.str(rhs.strsStream.str());
					} 

					~Stream_c()
					{
						// flush on destroy
						if (strsStream.tellp() > 0)
						{
							rclTarget.Message(strsStream.str());
						}
					}

					template <typename T>
					Stream_c& operator<< (const T& v)
					{
						strsStream << v;
						return *this;
					}

					Stream_c& operator<< (const Flush &)
					{								
						rclTarget.Message(strsStream.str());
						strsStream.str("");

						return *this;
					}

					Stream_c &operator<<(const Endl &)
					{
						strsStream << std::endl;

						return *this;
					}
			};

		public:
			Log_c(const String_c &name, UInt_t flags);
			~Log_c();

			void Message(const String_c &message);
			inline Stream_c Stream();

			void AddListener(LogListener_c &listener);
			void RemoveListener(LogListener_c &listener);
			size_t GetListenersCount() const;

		private:
			PH_DECLARE_LISTENER_LIST_TYPE(LogListener_c);

			std::ofstream		clFile;
			ListenersList_t		lstListeners;

			bool				fCopyToStdout;
	};

	inline Log_c::Stream_c Log_c::Stream()
	{
		return Stream_c(*this);
	}
}

#endif
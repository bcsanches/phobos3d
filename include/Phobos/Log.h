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

#include "Phobos/Defs.h"
#include "Phobos/DisableCopy.h"
#include "Phobos/Listener.h"
#include "Phobos/BaseAPI.h"
#include "Phobos/String.h"

namespace Phobos
{
	enum LogFlags_e
	{
		LOG_FLAG_COPY_TO_STDOUT = 0x01
	};
	

	class PH_BASE_API Log
	{
		PH_DISABLE_COPY(Log);

		public:
			class Listener
			{
				public:
					virtual ~Listener() {};

					virtual void OnLogMessage(const String_t &message) = 0;
			
					PH_DECLARE_LISTENER_HOOK
			};
		
			class Stream
			{
				protected:
					Log &m_rclTarget;					
					std::stringstream m_strsStream;					
		
				public:
					/// Simple type to indicate a flush of the stream to the log
					struct Flush {};
					struct Endl {};

					Stream(Log &target):
						m_rclTarget(target)
					{
						//empty
					}

					// copy constructor
					Stream(const Stream& rhs): 
						m_rclTarget(rhs.m_rclTarget)
					{
						m_strsStream.str(rhs.m_strsStream.str());
					} 

					~Stream()
					{
						// flush on destroy
						if (m_strsStream.tellp() > 0)
						{
							m_rclTarget.Message(m_strsStream.str());
						}
					}

					template <typename T>
					Stream& operator<< (const T& v)
					{
						m_strsStream << v;
						return *this;
					}

					Stream& operator<< (const Flush &)
					{								
						m_rclTarget.Message(m_strsStream.str());
						m_strsStream.str("");

						return *this;
					}

					Stream &operator<<(const Endl &)
					{
						m_strsStream << std::endl;

						return *this;
					}
			};

		public:
			Log(const String_t &name, UInt_t flags);
			~Log();

			void swap(Log &rhs);

			void Message(const String_t &message);
			inline Stream MakeStream();

			void AddListener(Listener &listener);
			void RemoveListener(Listener &listener);
			size_t GetListenersCount() const;

		private:
			PH_DECLARE_LISTENER_LIST_TYPE(Listener);

			std::ofstream		m_clFile;
			ListenersList_t		m_lstListeners;

			bool				m_fCopyToStdout;
	};

	inline Log::Stream Log::MakeStream()
	{
		return Stream(*this);
	}

	//
	//
	//GLOBAL LOG
	//
	//

	PH_BASE_API void LogMessage(const String_t &message);
	PH_BASE_API Log::Stream LogMakeStream();

	PH_BASE_API void LogAddListener(Log::Listener &listener);
	PH_BASE_API void LogRemoveListener(Log::Listener &listener);

	PH_BASE_API void LogChangeFile(const char *newFile);
}

#endif

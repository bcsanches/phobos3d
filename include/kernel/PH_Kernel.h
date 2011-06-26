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

#ifndef PH_KERNEL_H
#define PH_KERNEL_H


#include "PH_KernelAPI.h"
#include "PH_Log.h"
#include "PH_ObjectManager.h"
#include "PH_String.h"

#define PH_KERNEL_TYPE PH_OBJECT_TYPE_BUILD_TYPE(PH_VENDOR_IMMERSION, PH_ObjectType_c::KERNEL)


namespace Phobos
{
	/**

		\ingroup kernel

		The main class of Immersion, this class should be created before any other
		Immersion object.

		This class is used to manage memory and objects

	*/
	class PH_KERNEL_API Kernel_c
	{		
		public:				
			// =====================================================
			// KERNEL STATIC FUNCTIONS 
			// =====================================================

			//Creates a kernel instance, if ptrKernel isnt null, returns the instance.
			static Kernel_c &CreateInstance(const String_c &logName);

			//Used to retrieve the kernel pointer (Create instance must be called before calling this first time)		
			static Kernel_c &GetInstance(void);

			//destroy the current kernel instance, CreateInstance must be called before using it again
			static void ReleaseInstance(void);		

			// =====================================================
			// LOCAL METHODS
			// =====================================================
			inline void LogMessage(const String_c &message);
			inline Log_c::Stream_c LogStream();

			void AddLogListener(LogListener_c &listener);
			void RemoveLogListener(LogListener_c &listener);

			inline void AddObject(NodePtr_t ptr, const Path_c &path);
			inline NodePtr_t LookupObject(const Path_c &path) const;

		private:
			//Singleton, so constructor and destructor is private.
			Kernel_c(const String_c &logName);
			~Kernel_c(void);	

		private:
			static Kernel_c *pclInstance_g;

			Log_c clLog;

			ObjectManager_c clObjectManager;
	};

	// =====================================================
	// KERNEL INLNE METHODS
	// =====================================================	
	inline void Kernel_c::AddObject(NodePtr_t ptr, const Path_c &path)
	{
		clObjectManager.AddObject(ptr, path);
	}

	inline NodePtr_t Kernel_c::LookupObject(const Path_c &path) const
	{
		return clObjectManager.LookupObject(path);
	}

	inline Log_c::Stream_c Kernel_c::LogStream()
	{
		return clLog.Stream();
	}

	inline void Kernel_c::LogMessage(const String_c &message)
	{
		clLog.Message(message);
	}	
}

#endif

/*
Phobos 3d
September 2010
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
#include "PH_DynamicLibrary.h"
#include <iostream>

#include "PH_Exception.h"
#include "PH_Path.h"

#if (!defined PH_LINUX) && (!defined PH_WIN32)
#error "Phobos: Plataform not supported!"
#endif

#ifdef PH_LINUX
#include <dlfcn.h>
#define CloseLib dlclose

inline void *OpenLib(const char *name)
{
	return dlopen(name, RTLD_LAZY);
}

#define GetLibSymbol dlsym

#elif defined PH_WIN32
#include <windows.h>

inline void CloseLib(void *handle)
{
	FreeLibrary(static_cast<HMODULE>(handle));
}

#define OpenLib LoadLibrary
inline void *GetLibSymbol(void *handle, const char *name)
{
	return GetProcAddress(static_cast<HMODULE>(handle), name);
}

#endif

namespace Phobos
{
	DynamicLibrary_c::DynamicLibrary_c():
		pHandle(NULL)
	{
		//empty
	}

	DynamicLibrary_c::~DynamicLibrary_c()
	{
		if(pHandle != NULL)
            CloseLib(pHandle);
	}

	void DynamicLibrary_c::Load(const String_c &name)
	{
		strName = name;

		String_c tmp;
		Path_c::ConvertToPlatform(tmp, name);

		#ifdef PH_LINUX
            Path_c path(tmp);
            path.SetExtension(".so");
            tmp = path.GetStr();
        #endif

		if(pHandle != NULL)
			CloseLib(pHandle);

		pHandle = OpenLib(tmp.c_str());
		if(pHandle == NULL)
		{
			this->RaiseException("DynamicLibrary_c::Load");
		}
	}

	void *DynamicLibrary_c::TryGetSymbol(const String_c &name)
	{
		return GetLibSymbol(pHandle, name.c_str());
	}

	void *DynamicLibrary_c::GetSymbol(const String_c &name)
	{
		void *ptr = this->TryGetSymbol(name);
		if(ptr == NULL)
		{
			this->RaiseException("DynamicLibrary_c::GetSymbol");
		}

		return ptr;
	}

	void DynamicLibrary_c::RaiseException(const char *module)
	{
#if defined PH_LINUX
		String_c ret = dlerror();

		PH_RAISE(NATIVE_API_FAILED_EXCEPTION, module, ret);
#elif defined PH_WIN32
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0,
			NULL
		);

		String_c ret = (char*)lpMsgBuf;
		// Free the buffer.
		LocalFree( lpMsgBuf );

		PH_RAISE(NATIVE_API_FAILED_EXCEPTION, module, ret);
#endif
	}
}

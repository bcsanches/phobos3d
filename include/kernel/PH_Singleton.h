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

#ifndef PH_SINGLETON_H
#define PH_SINGLETON_H

#include "PH_Memory.h"

#define PH_DECLARE_SINGLETON_PTR(X)	PH_DECLARE_NODE_PTR(X)	

#define PH_DECLARE_SINGLETON_METHODS(X)		\
	private:								\
		static X##Ptr_t ipInstance_gl;		\
											\
	public:									\
		static X##_c &CreateInstance();		\
		static void ReleaseInstance();		\
		static X##_c &GetInstance();

#define PH_DECLARE_NAMED_SINGLETON_METHODS(X)					\
	private:													\
		static X##Ptr_t ipInstance_gl;							\
																\
	public:														\
		static X##_c &CreateInstance(const String_c &name);		\
		static void ReleaseInstance();							\
		static X##_c &GetInstance();

#define PH_DEFINE_SINGLETON_VAR(X) X##Ptr_t X##_c::ipInstance_gl;

#define PH_SINGLETON_PROCS(X, EXTRA)		\
	X##_c &X##_c::GetInstance(void)			\
	{										\
		return *ipInstance_gl;				\
	}										\
											\
	void X##_c::ReleaseInstance(void)		\
	{										\
		EXTRA;								\
		ipInstance_gl.reset();				\
	}

#define PH_DEFINE_DEFAULT_SINGLETON_EX(X, EXTRA_CREATE, EXTRA_RELEASE)\
	PH_DEFINE_SINGLETON_VAR(X)				\
	X##_c &X##_c::CreateInstance(void)		\
	{										\
		PH_ASSERT(!ipInstance_gl);			\
											\
		ipInstance_gl.reset(PH_NEW X##_c());	\
											\
		EXTRA_CREATE;						\
											\
		return *ipInstance_gl;				\
	}										\
	PH_SINGLETON_PROCS(X,EXTRA_RELEASE);

#define PH_DEFINE_NODE_SINGLETON(X, NODE_PATH) PH_DEFINE_DEFAULT_SINGLETON_EX(X, Kernel_c::GetInstance().AddObject(*ipInstance_gl, Path_c(NODE_PATH)), ipInstance_gl->RemoveSelf())	

#define PH_DEFINE_DEFAULT_SINGLETON(X) PH_DEFINE_DEFAULT_SINGLETON_EX(X, ;, ;)	

#endif
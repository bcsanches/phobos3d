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

#include "Phobos/DisableCopy.h"
#include "Phobos/Memory.h"
#include "Phobos/Node.h"

#define PH_DECLARE_SINGLETON_PTR(X)	PH_DECLARE_NODE_PTR(X)	

#define PH_DECLARE_SINGLETON_METHODS(X)		\
		PH_DISABLE_COPY(X)					\
	private:								\
		static X##Ptr_t ipInstance_gl;		\
											\
	public:									\
		static X &CreateInstance();    		\
		static void ReleaseInstance();		\
		static X &GetInstance();

#define PH_DECLARE_NAMED_SINGLETON_METHODS(X)					\
	private:													\
		static X##Ptr_t ipInstance_gl;							\
																\
	public:														\
		static X &CreateInstance(const String_t &name);			\
		static void ReleaseInstance();							\
		static X &GetInstance();

#define PH_DEFINE_SINGLETON_VAR(X) X##Ptr_t X::ipInstance_gl;

#define PH_SINGLETON_PROCS(X, EXTRA)		\
	X &X::GetInstance(void)					\
	{										\
		return *ipInstance_gl;				\
	}										\
											\
	void X::ReleaseInstance(void)			\
	{										\
		EXTRA;								\
		ipInstance_gl.reset();				\
	}

#define PH_DEFINE_DEFAULT_SINGLETON_EX(X, EXTRA_CREATE, EXTRA_RELEASE)\
	PH_DEFINE_SINGLETON_VAR(X)				\
	X &X::CreateInstance()					\
	{										\
		PH_ASSERT(!ipInstance_gl);			\
											\
		ipInstance_gl.reset(PH_NEW X());	\
											\
		EXTRA_CREATE;						\
											\
		return *ipInstance_gl;				\
	}										\
	PH_SINGLETON_PROCS(X,EXTRA_RELEASE);

#define PH_DEFINE_NODE_SINGLETON(X, NODE_PATH) PH_DEFINE_DEFAULT_SINGLETON_EX(X, Phobos::ObjectManager::AddObject(*ipInstance_gl, Path(NODE_PATH)), ipInstance_gl->RemoveSelf())	

#define PH_DEFINE_DEFAULT_SINGLETON(X) PH_DEFINE_DEFAULT_SINGLETON_EX(X, ;, ;)	
#define PH_DEFINE_DEFAULT_SINGLETON1(X) PH_DEFINE_DEFAULT_SINGLETON_EX(X, ;, ;)	

#define PH_DECLARE_SINGLETON_METHODS2(X,Y)	\
	PH_DISABLE_COPY(X)						\
											\
	public:									\
	static X &CreateInstance(Y);    		\
	static void ReleaseInstance();			\
	static X &GetInstance();

#define PH_DEFINE_DEFAULT_SINGLETON2(X, Y)	\
	static X *g_pclInstance = nullptr;		\
	X &X::CreateInstance(Y param)			\
	{										\
		PH_ASSERT(!g_pclInstance);			\
		g_pclInstance = PH_NEW X(param);	\
		return *g_pclInstance;				\
	}										\
	X &X::GetInstance()						\
	{										\
		return *g_pclInstance;				\
	}										\
	void X::ReleaseInstance()				\
	{										\
		delete g_pclInstance;				\
		g_pclInstance = nullptr;			\
	}


#endif

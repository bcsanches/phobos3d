/*
Phobos 3d
  May 2011

  Copyright (C) 2005-2011 Bruno Crivelari Sanches

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

#ifndef PH_ENTITY_IO_H
#define PH_ENTITY_IO_H

#include <map>

#include <boost/bind.hpp>
#include <boost/signal.hpp>

#include <PH_String.h>

namespace Phobos
{
	class EntityEvent_c;

	class EntityIO_c
	{
		public:
			EntityIO_c() {}
			virtual ~EntityIO_c() {}

			typedef boost::signal1<void, EntityEvent_c &> OutputSignal_t;
	};

	typedef void (EntityIO_c::*InputProc_t)(EntityEvent_c &);

	//Connects the given input to a signal
	typedef void (EntityIO_c::*OutputProcConnector_t)(EntityIO_c &ent, InputProc_t input);

	//Keeps a list with all Signals from a object
	class EntityOutputManager_c
	{
		public:	
			EntityOutputManager_c();

			void AddConnector(const String_c &name, OutputProcConnector_t proc);

			void Connect(EntityIO_c &outputOwner, const std::string &outputName, EntityIO_c &inputOwner, InputProc_t input);

		private:
			typedef std::map<String_c, OutputProcConnector_t> ConnectorsMap_t;
			ConnectorsMap_t mapConnectors;
	};

	class EntityInputManager_c
	{
		public:
			EntityInputManager_c();

			void AddSlot(const String_c &name, InputProc_t proc);

		private:
			typedef std::map<String_c, InputProc_t> InputMap_t;
			InputMap_t mapInputs;
	};	

	namespace EntityIO
	{
		class AutoOutputRegister_c
		{
			public:
				AutoOutputRegister_c(EntityOutputManager_c &manager, const String_c &outputName, OutputProcConnector_t proc)
				{
					manager.AddConnector(outputName, proc);
				}
		};
	}
}

#define PH_BEGIN_ENTITY_OUTPUT_DECLARATION(CLASS)				\
	class CLASS##OutputManager_c: public EntityOutputManager_c	\
	{															\
		public:													\
			CLASS##OutputManager_c();							\
	};

#define PH_DECLARE_ENTITY_OUTPUT(NAME)							\
	OutputSignal_t sig##NAME;									\
	void Connect_##NAME(EntityIO_c &other, InputProc_t proc);

#define PH_DEFINE_ENTITY_OUTPUT(CLASS, NAME)						\
	void CLASS::Connect_##NAME(EntityIO_c &other, InputProc_t proc)	\
	{																\
		sig##NAME.connect(boost::bind(proc, &other, _1));			\
	}

#define PH_END_ENTITY_OUTPUT_DECLARATION

#endif


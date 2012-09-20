/*
Phobos 3d
May 2011
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

#ifndef PH_ENTITY_IO_H
#define PH_ENTITY_IO_H

#include <map>

#include <boost/bind.hpp>
#include <boost/signal.hpp>

#include <PH_Node.h>
#include <PH_String.h>

#include "PH_GameEngineAPI.h"

namespace Phobos
{
	class EntityEvent_c;

	class PH_GAME_ENGINE_API EntityIO_c: public Node_c
	{
		protected:
			explicit EntityIO_c(const String_c &name, UInt32_t flags = 0);
			explicit EntityIO_c(const Char_t *name, UInt32_t flags = 0);
			
			~EntityIO_c();

		public:
			typedef boost::signal1<void, EntityEvent_c &> OutputSignal_t;

		public:
			void FixedUpdate();
			void Update();

		protected:
			virtual void OnFixedUpdate() {}
			virtual void OnUpdate() {}

			void EnableFixedUpdate();
			void EnableUpdate();
			void DisableFixedUpdate();
			void DisableUpdate();

		private:
			bool fFixedUpdateEnabled;
			bool fUpdateEnabled;
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

		class AutoInputRegister_c
		{
			public:
				AutoInputRegister_c(EntityInputManager_c &manager, const String_c &inputName, InputProc_t proc)
				{
					manager.AddSlot(inputName, proc);
				}
		};
	}
}

#define PH_DECLARE_ENTITY_INPUT(NAME)		\
	void Input_##NAME(EntityEvent_c &ev);	\
	static Phobos::EntityIO::AutoInputRegister_c clAutoInputRegister##NAME##_gl;

#define PH_BEGIN_ENTITY_INPUT(CLASS, NAME)																				\
	Phobos::EntityIO::AutoInputRegister_c CLASS::clAutoInputRegister##NAME##_gl(CLASS::clInputManager_gl, "##NAME", reinterpret_cast<InputProc_t>(&CLASS::Input_##NAME));	\
	void CLASS::Input_##NAME(EntityEvent_c &event)

#define PH_DECLARE_ENTITY_OUTPUT(NAME)												\
	OutputSignal_t sig##NAME;														\
	static Phobos::EntityIO::AutoOutputRegister_c clAutoOutputRegister##NAME##_gl;	\
	void Connect_##NAME(EntityIO_c &other, InputProc_t proc);

#define PH_DEFINE_ENTITY_OUTPUT(CLASS, NAME)																																\
	Phobos::EntityIO::AutoOutputRegister_c CLASS::clAutoOutputRegister##NAME##_gl(CLASS::GetOutputManager(), "##NAME", reinterpret_cast<OutputProcConnector_t>(&CLASS::Connect_##NAME));	\
	void CLASS::Connect_##NAME(EntityIO_c &other, InputProc_t proc)																											\
	{																																										\
		sig##NAME.connect(boost::bind(proc, &other, _1));																													\
	}

#define PH_DECLARE_ENTITY_OUTPUT_MANAGER static EntityOutputManager_c &GetOutputManager();
#define PH_DEFINE_ENTITY_OUTPUT_MANAGER(CLASS)				\
	EntityOutputManager_c& CLASS::GetOutputManager()		\
	{														\
		static EntityOutputManager_c clOutputManager_gl;	\
															\
		return clOutputManager_gl;							\
	}

#define PH_DECLARE_ENTITY_INPUT_MANAGER static EntityInputManager_c &GetInputManager();
#define PH_DEFINE_ENTITY_INPUT_MANAGER(CLASS)				\
	EntityInputManager_c& CLASS::GetInputManager()		\
	{														\
		static EntityInputManager_c clInputManager_gl;	\
															\
		return clInputManager_gl;							\
	}

#endif


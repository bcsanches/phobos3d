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

#include <Phobos/Node.h>
#include <Phobos/String.h>

#include "PH_GameEngineAPI.h"

namespace Phobos
{
	class EntityEvent;

	class PH_GAME_ENGINE_API EntityIO: public Node
	{
		protected:
			explicit EntityIO(const String_t &name, UInt32_t flags = 0);
			explicit EntityIO(const Char_t *name, UInt32_t flags = 0);
			
			~EntityIO();

		public:
			typedef boost::signal1<void, EntityEvent &> OutputSignal_t;

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

	typedef void (EntityIO::*InputProc_t)(EntityEvent &);

	//Connects the given input to a signal
	typedef void (EntityIO::*OutputProcConnector_t)(EntityIO &ent, InputProc_t input);

	//Keeps a list with all Signals from a object
	class EntityOutputManager
	{
		public:
			EntityOutputManager();

			void AddConnector(const String_t &name, OutputProcConnector_t proc);

			void Connect(EntityIO &outputOwner, const std::string &outputName, EntityIO &inputOwner, InputProc_t input);

		private:
			typedef std::map<String_t, OutputProcConnector_t> ConnectorsMap_t;
			ConnectorsMap_t mapConnectors;
	};

	class EntityInputManager
	{
		public:
			EntityInputManager();

			void AddSlot(const String_t &name, InputProc_t proc);

		private:
			typedef std::map<String_t, InputProc_t> InputMap_t;
			InputMap_t mapInputs;
	};
	
	class AutoOutputRegister
	{
		public:
			AutoOutputRegister(EntityOutputManager &manager, const String_t &outputName, OutputProcConnector_t proc)
			{
				manager.AddConnector(outputName, proc);
			}
	};

	class AutoInputRegister
	{
		public:
			AutoInputRegister(EntityInputManager &manager, const String_t &inputName, InputProc_t proc)
			{
				manager.AddSlot(inputName, proc);
			}
	};	
}

#define PH_DECLARE_ENTITY_INPUT(NAME)		\
	void Input_##NAME(EntityEvent &ev);	\
	static Phobos::AutoInputRegister m_clAutoInputRegister##NAME##_gl;

#define PH_BEGIN_ENTITY_INPUT(CLASS, NAME)																				\
	Phobos::AutoInputRegister CLASS::m_clAutoInputRegister##NAME##_gl(CLASS::clInputManager_gl, "##NAME", reinterpret_cast<InputProc_t>(&CLASS::Input_##NAME));	\
	void CLASS::Input_##NAME(EntityEvent &event)

#define PH_DECLARE_ENTITY_OUTPUT(NAME)												\
	OutputSignal_t sig##NAME;														\
	static Phobos::AutoOutputRegister m_clAutoOutputRegister##NAME##_gl;	\
	void Connect_##NAME(EntityIO &other, InputProc_t proc);

#define PH_DEFINE_ENTITY_OUTPUT(CLASS, NAME)																																\
	Phobos::AutoOutputRegister CLASS::m_clAutoOutputRegister##NAME##_gl(CLASS::GetOutputManager(), "##NAME", reinterpret_cast<OutputProcConnector_t>(&CLASS::Connect_##NAME));	\
	void CLASS::Connect_##NAME(EntityIO &other, InputProc_t proc)																											\
	{																																										\
		sig##NAME.connect(boost::bind(proc, &other, _1));																													\
	}

#define PH_DECLARE_ENTITY_OUTPUT_MANAGER static EntityOutputManager &GetOutputManager();
#define PH_DEFINE_ENTITY_OUTPUT_MANAGER(CLASS)				\
	EntityOutputManager& CLASS::GetOutputManager()		\
	{														\
		static EntityOutputManager clOutputManager_gl;	\
															\
		return clOutputManager_gl;							\
	}

#define PH_DECLARE_ENTITY_INPUT_MANAGER static EntityInputManager &GetInputManager();
#define PH_DEFINE_ENTITY_INPUT_MANAGER(CLASS)				\
	EntityInputManager& CLASS::GetInputManager()		\
	{														\
		static EntityInputManager clInputManager_gl;	\
															\
		return clInputManager_gl;							\
	}

#endif


/*
Phobos 3d
  January 2011

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

#ifndef PH_GENERIC_FACTORY_H
#define PH_GENERIC_FACTORY_H

#include <boost/noncopyable.hpp>
#include <boost/intrusive/set.hpp>

#include <PH_Defs.h>
#include <PH_Exception.h>
#include <PH_String.h>

namespace Phobos
{
	typedef boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> > ObjectCreatorAutoUnlinkHook_t;	

	template <typename T>
	class ObjectCreator_c: public ObjectCreatorAutoUnlinkHook_t
	{
		public:
			typedef T ObjectType_t;
			typedef ObjectType_t (*ObjectCreatorProc_t)(const String_c &name);

		public:
			ObjectCreator_c(const String_c &name, ObjectCreatorProc_t proc);

			T Create(const String_c &name) const;

			inline const String_c &GetName() const
			{
				return strName;
			}

			inline bool operator<(const ObjectCreator_c &rhs) const
            {
                return strName.compare(rhs.strName) < 0;    
            }

		private:
			String_c strName;
			ObjectCreatorProc_t pfnCreateProc;
	};

	template <typename T>
	class GenericFactory_c: boost::noncopyable
	{
		public:
			static GenericFactory_c &GetInstance();

			T Create(const String_c &className, const String_c &name) const;			

		private:
			GenericFactory_c();

			friend class ObjectCreator_c<T>;
			void Register(ObjectCreator_c<T> &creator);

		private:
			typedef boost::intrusive::set<ObjectCreator_c<T>, boost::intrusive::constant_time_size<false> > ObjectCreatorSet_t;
            ObjectCreatorSet_t setObjectCreators;
	};	

	template<typename T>
	struct ObjectCreatorComp_s
    {
        bool operator()(const String_c &name, const ObjectCreator_c<T> &res) const
        {
            return name.compare(res.GetName()) < 0;    
        }
 
        bool operator()(const ObjectCreator_c<T> &res, const String_c &name) const
        {
            return res.GetName().compare(name) < 0;
        }
    };

	template<typename T>
	ObjectCreator_c<T>::ObjectCreator_c(const String_c &name, ObjectCreatorProc_t proc):
		strName(name),
		pfnCreateProc(proc)
	{
		GenericFactory_c<T>::GetInstance().Register(*this);

		if(proc == NULL)
		{
			std::stringstream stream;
			stream << "creator proc cant be null, entity " << name;
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[ObjectCreator_c::ObjectCreator_c]", stream.str());
		}
	}

	template<typename T>
	T ObjectCreator_c<T>::Create(const String_c &name) const
	{
		return pfnCreateProc(name);
	}
	
	template <typename T>
	GenericFactory_c<T> &GenericFactory_c<T>::GetInstance()
	{
		static GenericFactory_c<T> clInstance_gl;

		return clInstance_gl;
	}

	template <typename T>
	GenericFactory_c<T>::GenericFactory_c()
	{
		//empty
	}

	template <typename T>
	void GenericFactory_c<T>::Register(ObjectCreator_c<T> &creator)
	{
		setObjectCreators.insert(creator);
	}

	template <typename T>
	T GenericFactory_c<T>::Create(const String_c &className, const String_c &name) const
	{
		ObjectCreatorSet_t::const_iterator it = setObjectCreators.find(className, ObjectCreatorComp_s<T>());
		if(it == setObjectCreators.end())
			PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "[EntityFactory_c::Create]", name);

		return it->Create(name);
	}
}



#endif

#ifndef PH_EDITOR_REQUEST_FACTORY_H
#define PH_EDITOR_REQUEST_FACTORY_H

#include <Phobos/GenericFactory.h>
#include <Phobos/Memory.h>

#include <rapidjson/document.h>

#include "Phobos/Editor/Request.h"

namespace Phobos
{
	namespace Editor
	{	
		class RequestFactory: public GenericFactory1<ObjectCreator1<Request, rapidjson::Value, RequestFactory, std::unique_ptr<Phobos::Editor::Request>>, rapidjson::Value >
		{
			public:
				static RequestFactory &GetInstance();			
		};
	}
}

#define PH_REQUEST_CREATOR(NAME, TYPE, PROC)										\
	static Phobos::ObjectCreator1<Phobos::Editor::Request, rapidjson::Value, Phobos::Editor::RequestFactory, std::unique_ptr<Phobos::Editor::Request>> TYPE##_CreatorObject_gl(NAME, PROC);


#define PH_FULL_REQUEST_CREATOR(NAME, TYPE)  							\
	PH_REQUEST_CREATOR(NAME, TYPE, [](const rapidjson::Value &obj)		\
		{																\
			return std::unique_ptr<Request>(PH_NEW TYPE(obj));		\
		}																\
	);

#endif

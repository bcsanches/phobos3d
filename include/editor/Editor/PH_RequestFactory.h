#ifndef PH_EDITOR_REQUEST_FACTORY_H
#define PH_EDITOR_REQUEST_FACTORY_H

#include <PH_GenericFactory.h>
#include <PH_Memory.h>

#include <rapidjson/document.h>

#include "Editor/PH_Request.h"

namespace Phobos
{
	namespace Editor
	{	
		class RequestFactory_c: public GenericFactory1_c<ObjectCreator1_c<Request_c, rapidjson::Value, RequestFactory_c, std::unique_ptr<Phobos::Editor::Request_c>>, rapidjson::Value >
		{
			public:
				static RequestFactory_c &GetInstance();			
		};
	}
}

#define PH_REQUEST_CREATOR(NAME, TYPE, PROC)										\
	static Phobos::ObjectCreator1_c<Phobos::Editor::Request_c, rapidjson::Value, Phobos::Editor::RequestFactory_c, std::unique_ptr<Phobos::Editor::Request_c>> TYPE##_CreatorObject_gl(NAME, PROC);


#define PH_FULL_REQUEST_CREATOR(NAME, TYPE)  							\
	PH_REQUEST_CREATOR(NAME, TYPE, [](const rapidjson::Value &obj)		\
		{																\
			return std::unique_ptr<Request_c>(PH_NEW TYPE(obj));		\
		}																\
	);

#endif

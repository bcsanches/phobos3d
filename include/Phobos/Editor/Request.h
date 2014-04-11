#ifndef PH_EDITOR_REQUEST_H
#define PH_EDITOR_REQUEST_H

#include <rapidjson/document.h>
#include <JsonCreator/StringWriterFwd.h>
#include <JsonCreator/Object.h>

#include <Phobos/Types.h>

namespace Phobos
{
	namespace Editor
	{		
		class Request
		{
			public:
				Request(const rapidjson::Value &value);

				void Execute(const rapidjson::Value &request, JsonCreator::StringWriter &response);

				inline UInt_t GetId() const { return m_uId; }

			private:
				virtual void OnExecute(const rapidjson::Value *parameters, JsonCreator::Object<JsonCreator::StringWriter> *response) = 0;

			private:
				UInt_t	m_uId;
				
				bool	m_fNotification;
		};
	}
}

#endif

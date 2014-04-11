#ifndef PH_EDITOR_ERROR_REQUEST_H
#define PH_EDITOR_ERROR_REQUEST_H

#include "Phobos/Editor/Request.h"

namespace Phobos
{
	namespace Editor
	{
		class ErrorRequest: public Request
		{
			public:
				enum Errors
				{
					PARSE_ERROR = -32700,
					INVALID_REQUEST = -32600,
					METHOD_NOT_FOUND = -32601,
					INVALID_PARAMETERS = -32602,
					INTERNAL_ERROR = -32603
				};

			public:
				ErrorRequest(const rapidjson::Value &value, const char *message, Errors error);

			private:
				virtual void OnExecute(const rapidjson::Value *parameters, JsonCreator::Object<JsonCreator::StringWriter> *response) override;

			private:
				std::string m_strMessage;
				Errors		m_eError;
		};
	}
}

#endif

#ifndef PH_EDITOR_REQUEST_H
#define PH_EDITOR_REQUEST_H

#include <rapidjson/document.h>

namespace Phobos
{
	namespace Editor
	{
		class Request_c
		{
			public:
				Request_c(const rapidjson::Value &value);

				void Execute();

			private:
				virtual void OnExecute() = 0;
		};
	}
}

#endif

#ifndef PH_EDITOR_REQUEST_H
#define PH_EDITOR_REQUEST_H

#include <json_spirit.h>

namespace Phobos
{
	namespace Editor
	{
		class Request_c
		{
			public:
				Request_c(const json_spirit::mObject &value);

				void Execute();

			private:
				virtual void OnExecute() = 0;
		};
	}
}

#endif

#ifndef PH_EDITOR_ASSET_LIST_REQUEST_H
#define PH_EDITOR_ASSET_LIST_REQUEST_H

#include "Phobos/Editor/Request.h"

namespace Phobos
{
	namespace Editor
	{
		class AssetListRequest: public Request
		{
			public:
				AssetListRequest(const rapidjson::Value &value);

			private:
				virtual void OnExecute(JsonCreator::StringWriter &response) override;
		};
	}
}

#endif

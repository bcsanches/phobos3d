#ifndef PH_EDITOR_ASSET_LIST_REQUEST_H
#define PH_EDITOR_ASSET_LIST_REQUEST_H

#include "Editor/PH_Request.h"

namespace Phobos
{
	namespace Editor
	{
		class AssetListRequest_c: public Request_c
		{
			public:
				AssetListRequest_c(const rapidjson::Value &value);

			private:
				virtual void OnExecute(JsonCreator::StringWriter &response) override;
		};
	}
}

#endif

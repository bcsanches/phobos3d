#include "Phobos/Editor/Request.h"
#include "Phobos/Editor/RequestFactory.h"

#include <Phobos/Engine/Kernel.h>

namespace Editor = Phobos::Editor;

namespace
{
	class QuitRequest : public Editor::Request
	{
		public:
			QuitRequest(const rapidjson::Value &value);

		private:
			virtual void OnExecute(const rapidjson::Value *parameters, JsonCreator::Object<JsonCreator::StringWriter> *response) override;
	};

	PH_FULL_REQUEST_CREATOR("Quit", QuitRequest);

	QuitRequest::QuitRequest(const rapidjson::Value &value) :
		Request(value)
	{
		//empty
	}

	void QuitRequest::OnExecute(const rapidjson::Value *parameters, JsonCreator::Object<JsonCreator::StringWriter> *response)
	{
		Phobos::Engine::Kernel::GetInstance().StopMainLoop();
	}
}

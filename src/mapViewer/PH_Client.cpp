#include "PH_Client.h"

#include <PH_Error.h>

namespace Phobos
{
	PH_DEFINE_DEFAULT_SINGLETON(Client);

	Client_c::Client_c():
		CoreModule_c("Client")
	{
	}
}

